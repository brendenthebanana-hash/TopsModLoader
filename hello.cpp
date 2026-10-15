#include "tops.h"
#include <string>

extern "C" {
static const TopsPluginInfo info = {"hello", "0.1.0", "*"};
__attribute__((visibility("default"))) const TopsPluginInfo* tops_plugin_info() { return &info; }
__attribute__((visibility("default"))) void tops_plugin_init(const TopsApi* api) {
    api->log("hello", (std::string("Hello from Tops! game=") + api->game_version).c_str());
}
}
