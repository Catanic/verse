# Building on Steam Deck with Tracy

The Steam Deck runs a Linux-based system, so the project can be built
natively. These steps enable the [Tracy profiler](https://github.com/wolfpld/tracy)
for performance analysis.

## Prerequisites

Enable developer mode on the Deck and install build tools and libraries
including Mesa for OpenGL headers:

```bash
sudo pacman -S --needed base-devel cmake git \
    sdl2 sdl2_image sdl2_mixer sdl2_ttf sdl2_net sdl2_gfx \
    libpng zlib freetype2 harfbuzz libxml2 curl mesa
```

## Building

Run the helper script to configure and compile with Tracy support:

```bash
./tools/build_steamdeck_tracy.sh
```

The compiled client is located at
`build-steamdeck-tracy/src/manaplus`.

## Profiling with Tracy

1. Download and run the Tracy capture server from the
   [Tracy releases](https://github.com/wolfpld/tracy/releases) page
   on your Deck or a separate PC.
2. Start the client:
   ```bash
   ./build-steamdeck-tracy/src/manaplus
   ```
3. The client connects automatically to the Tracy server on `localhost`.
   If the server runs on another machine, set the `TRACY_HOST`
   environment variable to its address before launching the client.
