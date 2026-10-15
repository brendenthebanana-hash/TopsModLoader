// tops-core: loaded by mcpelauncher as a normal mod. Loads Tops plugins.
#include "../api/tops.h"

#include <dlfcn.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <vector>

namespace fs = std::filesystem;

static std::vector<void*> g_loaded;
static std::string g_game_version = "unknown";

static void tops_log(const char* plugin, const char* msg) {
    std::cout << "[Tops:" << plugin << "] " << msg << std::endl;
}

static fs::path data_dir() {
    // Flatpak sets XDG_DATA_HOME to ~/.var/app/<id>/data inside the sandbox.
    const char* xdg = getenv("XDG_DATA_HOME");
    if (xdg && *xdg) return fs::path(xdg) / "mcpelauncher";
    const char* home = getenv("HOME");
    return fs::path(home ? home : ".") / ".local/share/mcpelauncher";
}

static bool version_ok(const char* supported) {
    if (!supported || !strcmp(supported, "*")) return true;
    if (g_game_version == "unknown") return true;
    std::istringstream ss(supported);
    std::string v;
    while (ss >> v) if (v == g_game_version) return true;
    return false;
}

extern "C" __attribute__((visibility("default"))) void mod_preinit() {}

extern "C" __attribute__((visibility("default"))) void mod_init() {
    if (const char* gv = getenv("TOPS_GAME_VERSION")) g_game_version = gv;
    static TopsApi api = {TOPS_API_VERSION, tops_log, nullptr, nullptr};
    api.game_version = g_game_version.c_str();

    fs::path dir = data_dir() / "tops" / "plugins";
    std::error_code ec;
    if (!fs::exists(dir, ec)) {
        std::cout << "[Tops] no plugin dir at " << dir << std::endl;
        return;
    }

    std::vector<fs::path> files;
    for (auto& e : fs::directory_iterator(dir, ec))
        if (e.path().extension() == ".so") files.push_back(e.path());
    std::sort(files.begin(), files.end());  // load order: alphabetical

    for (auto& p : files) {
        void* h = dlopen(p.c_str(), RTLD_NOW | RTLD_LOCAL);
        if (!h) { std::cerr << "[Tops] failed " << p.filename() << ": " << dlerror() << std::endl; continue; }

        auto info_fn = (const TopsPluginInfo* (*)())dlsym(h, "tops_plugin_info");
        auto init_fn = (void (*)(const TopsApi*))dlsym(h, "tops_plugin_init");
        if (!info_fn || !init_fn) {
            std::cerr << "[Tops] " << p.filename() << " missing tops_plugin_info/init" << std::endl;
            dlclose(h);
            continue;
        }
        const TopsPluginInfo* info = info_fn();
        if (!version_ok(info->game_versions)) {
            std::cout << "[Tops] skipping " << info->name << " (needs " << info->game_versions
                      << ", have " << g_game_version << ")" << std::endl;
            dlclose(h);
            continue;
        }
        init_fn(&api);
        g_loaded.push_back(h);
        std::cout << "[Tops] loaded " << info->name << " " << info->version << std::endl;
    }
}
