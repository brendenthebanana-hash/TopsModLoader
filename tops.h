// Tops plugin API (v1). Include this in your plugin.
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#define TOPS_API_VERSION 1

typedef struct TopsApi {
    int api_version;
    // Print a line to the launcher log, prefixed with the plugin name.
    void (*log)(const char* plugin, const char* msg);
    // Game version string if known, otherwise "unknown".
    const char* game_version;
    // Reserved for hook helpers; wire these to the launcher's hook API
    // (see README, "Hooking"). NULL until implemented.
    void* (*hook)(void* target, void* replacement);
} TopsApi;

typedef struct TopsPluginInfo {
    const char* name;
    const char* version;
    // Space separated game versions this plugin supports, or "*" for any.
    const char* game_versions;
} TopsPluginInfo;

// Every plugin exports these two symbols:
//   const TopsPluginInfo* tops_plugin_info(void);
//   void tops_plugin_init(const TopsApi* api);

#ifdef __cplusplus
}
#endif
