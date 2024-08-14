#!/bin/bash

if [ -d "External/symengine" ]; then
    echo "External/symengine already exists. Skipping download."
else
    echo "External/symengine does not exist. Cloning symengine repository..."
    git clone https://github.com/symengine/symengine.git External/symengine
fi

cd External/symengine/

# MacOS combined

mkdir build_macOS_combined
cd build_macOS_combined
cmake -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
      -DMACOSX_DEPLOYMENT_TARGET="15.0" \
      -DGMP_INCLUDE_DIR=/usr/local/opt/gmp/include \
      -DGMP_LIBRARY=../../../libs/libgmp_macOS.a \
      -DGMPXX_LIBRARY=../../../libs/libgmpxx_macOS.a \
      -DWITH_GMPXX=ON \
      ..

make -j 12

cd ../

# iOS arm64

export IPHONEOS_DEPLOYMENT_TARGET="15.0"

mkdir build_ios
cd build_ios
cmake -DCMAKE_TOOLCHAIN_FILE=../../../toolchains/Toolchain-iPhoneOS_Xcode.cmake \
      -DPLATFORM=OS64 \
      -DIOS_ARCH=arm64 \
      -DIPHONEOS_DEPLOYMENT_TARGET="15.0" \
      -DCMAKE_OSX_ARCHITECTURES="arm64" \
      -DGMP_INCLUDE_DIR=/usr/local/opt/gmp/include \
      -DGMP_LIBRARY=../../../libs/libgmp.a \
      -DGMPXX_LIBRARY=../../../libs/libgmpxx.a \
      -DWITH_GMPXX=ON \
      ..

make -j 12

cd ../

cp build_iOS_arm64/symengine/libsymengine.a ../../libs/libsymengine.a
cp build_macOS_combined/libsymengine.a ../../libs/libsymengine_macOS.a