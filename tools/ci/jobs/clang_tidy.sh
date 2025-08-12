#!/usr/bin/env bash

export LOGFILE=tidy.log

source ./tools/ci/scripts/init.sh

rm "logs/clang_tidy.txt"

cd src

find . -type f -name "*.cpp" -exec ../tools/ci/scripts/clang_tidy.sh "${FLAGS}" "${DFLAGS}" {} \; >>../logs/clang_tidy.txt
check_error $?
find . -type f -name "*.cc" -exec ../tools/ci/scripts/clang_tidy.sh "${FLAGS}" "-DUNITTESTS -DUNITTESTS_CATCH ${DFLAGS}" {} \; >>../logs/clang_tidy.txt
check_error $?
find . -type f -name "*.h" -exec ../tools/ci/scripts/clang_tidy.sh "${FLAGS}" "${DFLAGS}" {} \; >>../logs/clang_tidy.txt
check_error $?

cd ..

grep "warning:" logs/clang_tidy.txt \
  | grep -v \
         -e "/src/unittests/doctest.h" \
         -e "/src/unittests/catch.hpp" \
         -e "/src/sdl2gfx/" \
         -e "/src/debug/" \
    ">${ERRFILE}"

run_check_warnings

echo ok
