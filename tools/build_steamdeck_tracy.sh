#!/bin/bash
set -euo pipefail

# Build Mana Verse Client with Tracy profiling support on Steam Deck.
# This script installs required dependencies and then configures and
# compiles the project using SDL2 and Tracy.
# Usage: ./tools/build_steamdeck_tracy.sh

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build-steamdeck-tracy"

# Refuse to run as root so pkg-config can locate the Deck's libraries
if (( EUID == 0 )); then
  echo "Please run this script without sudo; it will use sudo when installing packages." >&2
  exit 1
fi

# Packages needed to build on Steam Deck
PACKAGES=(
  base-devel cmake git
  sdl2 sdl2_image sdl2_mixer sdl2_ttf sdl2_net sdl2_gfx
  libpng zlib freetype2 harfbuzz libxml2 curl
  mesa libglvnd glu
  libjxl libjpeg-turbo libtiff libavif libwebp
  bzip2 brotli glib2 graphite libidn2 zstd krb5 openssl
  libpsl libssh2 libnghttp2 libnghttp3 xz icu
)

# Install dependencies with pacman if available
if command -v pacman >/dev/null 2>&1; then
  sudo pacman -S --needed --noconfirm "${PACKAGES[@]}"
else
  echo "Warning: pacman not found. Ensure the following packages are installed:" >&2
  echo "  ${PACKAGES[*]}" >&2
fi

# Ensure pkg-config searches the system directories
export PKG_CONFIG_PATH="${PKG_CONFIG_PATH:-/usr/lib/pkgconfig:/usr/share/pkgconfig}"

cmake -S "$ROOT_DIR" -B "$BUILD_DIR" \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DENABLE_TRACY=ON \
  -DUSE_SDL2=ON

cmake --build "$BUILD_DIR" -j"$(nproc)"
