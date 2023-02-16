#!/bin/sh
cd "$(dirname "$0")"
rm -rf build
mkdir build
cd build
/opt/mxe/usr/bin/x86_64-w64-mingw32.static-cmake -DCMAKE_BUILD_TYPE=Release -DDEV_BUILD=OFF -DBUILD_PACKAGE=OFF .. \
 && make -j4 \
 && cd bin && /opt/mxe/usr/bin/x86_64-w64-mingw32.static-strip -d openxcom.exe
