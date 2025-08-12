#!/usr/bin/env bash

if [[ "$RUNFROMSHELL" != "" ]]; then
    if [[ ${CIRRUS_CI} != "true" ]]; then
        echo "Running from shell. Skipping run tests $*"
        exit 0
    fi
fi

export SDL_VIDEODRIVER="dummy"
ulimit -c unlimited -S
ulimit -c unlimited
rm -rf core*
sysctl -w kernel.core_pattern=core

export HOME="logs/home"
rm -rf $HOME

export ASAN_OPTIONS=detect_stack_use_after_return=true:strict_init_order=true

echo "clean config runs"
n=0
while true; do
    echo "run test ${n}"
    rm "${HOME}/.config/mana/mana/config.xml"
    rm "/root/.config/mana/mana/config.xml"
    rm "${HOME}/.config/mana/mana/serverlistplus.xml"
    rm "/root/.config/mana/mana/serverlistplus.xml"
    rm "${HOME}/.local/share/mana/manaverse.log"
    rm "/root/.local/share/mana/manaverse.log"
    ./tools/ci/scripts/runtest.sh
    if [ "$?" != 0 ]; then
        exit 1
    fi
    if [[ $n -ge 2 ]]; then # Do a second test just to be on the safe side
        break
    fi
    sleep 5
    n=$((n+1))
done

exit 0

echo "existing config runs"
n=0
while true; do
    echo "run test ${n}"
    rm "${HOME}/.local/share/mana/manaverse.log"
    rm "/root/.local/share/mana/manaverse.log"
    ./tools/ci/scripts/runtest.sh
    if [ "$?" != 0 ]; then
        exit 1
    fi
    if [[ $n -ge 1 ]]; then
        exit 0
    fi
    sleep 5
    n=$((n+1))
done
