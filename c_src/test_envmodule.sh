#!/bin/sh
cd build/lib.macosx-10.9-x86_64-2.7/
rm -rf test_envmodule.py
cp ../../test_envmodule.py ./
python2 test_envmodule.py
