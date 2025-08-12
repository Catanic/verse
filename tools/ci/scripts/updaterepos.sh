#!/usr/bin/env bash

if [[ "$RUNFROMSHELL" != "" ]]; then
    echo "Running from shell. Skipping update repos"
    exit 0
fi

if grep "nameserver 1.10.100.101" /etc/resolv.conf; then
    echo "Detected local runner"
    sed -i 's!http://httpredir.debian.org/debian!http://1.10.100.103/debian!' /etc/apt/sources.list || true
    sed -i 's!http://deb.debian.org/debian!http://1.10.100.103/debian!' /etc/apt/sources.list || true
fi
