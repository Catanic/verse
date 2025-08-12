#!/usr/bin/env bash

echo "CI nodes: ${CIRCLE_NODE_TOTAL}"
echo "Current node: ${CIRCLE_NODE_INDEX}"

case $CIRCLE_NODE_INDEX in
    0)
        export IMAGE=debian:stretch
        export COMMAND="./tools/ci/jobs/gcc6_sanitize_tests.sh --enable-unittests=yes --without-dyecmd --without-gameclient --without-opengl --with-sdl2"
        export PACKAGES="gcc-6 g++-6 ${LIBS_PACKAGES} ${SDL2} valgrind"
        ./tools/ci/scripts/docker_run.sh
    ;;
    1)
        echo nothing
    ;;
    2)
        export IMAGE=debian:stretch
        export COMMAND="./tools/ci/jobs/gcc6_sanitize_tests.sh --enable-unittests=yes --without-dyecmd --without-gameclient --without-opengl"
        export PACKAGES="gcc-6 g++-6 ${LIBS_PACKAGES} ${SDL1} valgrind"
        ./tools/ci/scripts/docker_run.sh
    ;;
    3)
        echo nothing
    ;;
    *)
        echo unknown node
        exit 1
    ;;
esac
