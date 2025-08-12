cmake_minimum_required(VERSION 3.16)
project(TracyProfiler)

option(TRACY_USE_WAYLAND "Use Wayland backend" OFF)
if(TRACY_USE_WAYLAND)
    include(FindPkgConfig)
    pkg_check_modules(WAYLAND REQUIRED egl wayland-egl wayland-cursor xkbcommon)
    # Wayland-specific configuration goes here
endif()

add_library(TracyImGui STATIC)
find_package(Freetype REQUIRED)
target_include_directories(TracyImGui PRIVATE ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(TracyImGui PRIVATE ${FREETYPE_LIBRARIES})
