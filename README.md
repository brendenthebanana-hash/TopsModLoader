# Tops

A small mod loader for the Flathub **Minecraft Bedrock Launcher** (`io.mrarm.mcpelauncher`).
`tops-core` is loaded by the launcher as a normal mod and loads Tops plugins; the `tops` CLI manages files.
Nothing in the launcher or game is modified on disk.

## Build
    cmake -S . -B build && cmake --build build
(or `nix develop` first, or `nix build`)

## Install
    ./cli/tops setup --core build/libtops-core.so
    ./cli/tops install build/libtops-hello.so
    ./cli/tops status

Then launch the game and check the log for `[Tops] loaded hello 0.1.0`.

## Commands
`setup`, `install <.so>`, `remove|enable|disable <name>`, `list`, `status`.
Set `TOPS_DATA_DIR` to override the data directory.

## Writing a plugin
Include `api/tops.h` and export `tops_plugin_info` and `tops_plugin_init` (see `examples/hello`).
Set `game_versions` to `"*"` or a space separated version list. Export `TOPS_GAME_VERSION`
in the launcher environment if you want version checks to be enforced.

## Hooking (TODO)
`TopsApi.hook` is a placeholder. Wire it to the hook functions that mcpelauncher exposes to mods
(check the current mcpelauncher source for the exact symbols) before writing game patches.

## Flatpak notes
Build with static libstdc++/libgcc (already set in CMakeLists) or in the Freedesktop SDK,
and match the game's architecture (x86_64 / arm64).
