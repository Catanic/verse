#!/bin/bash

cd ..

DSTDIR=packaging/build/linux

export CXXFLAGS="-Wall -g0 -O3"
autoreconf -i
#make clean
./configure --enable-portable=yes
result=$?
if [ "$result" != 0 ]; then
    exit $result
fi

make -j16
result=$?
if [ "$result" != 0 ]; then
    exit $result
fi

if [ -e src/manaplus ];
then
    strip src/manaplus
    rm -rf $DSTDIR
    mkdir -p $DSTDIR
    cp -r data $DSTDIR
    mkdir -p $DSTDIR/bin
    cp src/manaplus $DSTDIR/bin
    cp nightly/runme.sh $DSTDIR
    cp "nightly/run tests.sh" $DSTDIR
fi
