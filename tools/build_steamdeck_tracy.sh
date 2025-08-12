#!/bin/bash
set -euo pipefail

# Build Mana Verse Client with Tracy profiling support on Steam Deck.
# This script configures and compiles the project using SDL2 and Tracy.
# Usage: ./tools/build_steamdeck_tracy.sh

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build-steamdeck-tracy"

cmake -S "$ROOT_DIR" -B "$BUILD_DIR" \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DENABLE_TRACY=ON \
  -DUSE_SDL2=ON

cmake --build "$BUILD_DIR" -j"$(nproc)"
