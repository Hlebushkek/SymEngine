#!/bin/bash

if [ -d "External/gmp" ]; then
    echo "External/gmp already exists. Skipping download."
else
    echo "External/gmp does not exist. Cloning GMP repository..."
    hg clone https://gmplib.org/repo/gmp-6.3/ External/gmp
fi

cd External/gmp

./.bootstrap

# MacOS x86_64
export SDKROOT=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk

export CFLAGS=" -O2 -fno-stack-check -isysroot ${SDKROOT} --target=x86_64-apple-darwin"
export CPPFLAGS=" -O2 -fno-stack-check -isysroot ${SDKROOT} --target=x86_64-apple-darwin"
export LDFLAGS=" -isysroot ${SDKROOT}"

./configure --enable-fat --enable-cxx --build=x86_64-apple-darwin --host=x86_64-apple-darwin --with-pic

make -j 12

mv .libs libs_macOS_x86_64

make clean

# MacOS arm64

export CFLAGS=" -O2 -fno-stack-check -isysroot ${SDKROOT} --target=arm64-apple-darwin"
export CPPFLAGS=" -O2 -fno-stack-check -isysroot ${SDKROOT} --target=arm64-apple-darwin"
export LDFLAGS=" -isysroot ${SDKROOT} -arch arm64"

./configure --enable-fat --enable-cxx --build=x86_64-apple-darwin --host=arm64-apple-darwin --with-pic

make -j 12

mv .libs libs_macOS_arm64

make clean

# Merge macOS libs

mkdir .libs
lipo -create -output .libs/libgmp_macOS.10.dylib libs_macOS_x86_64/libgmp.10.dylib libs_macOS_arm64/libgmp.10.dylib
lipo -create -output .libs/libgmp_macOS.a libs_macOS_x86_64/libgmp.a libs_macOS_arm64/libgmp.a

lipo -create -output .libs/libgmpxx_macOS.4.dylib libs_macOS_x86_64/libgmpxx.4.dylib libs_macOS_arm64/libgmpxx.4.dylib
lipo -create -output .libs/libgmpxx_macOS.a libs_macOS_x86_64/libgmpxx.a libs_macOS_arm64/libgmpxx.a

mv .libs libs_macOS_combined

cp libs_macOS_combined/libgmp_macOS.a ../../libs/libgmp_macOS.a
cp libs_macOS_combined/libgmpxx_macOS.a ../../libs/libgmpxx_macOS.a

# # iOS arm64

export SDKROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk
export CFLAGS=" -O2 -fno-stack-check -isysroot ${SDKROOT} -miphoneos-version-min=15.0 --target=arm64-apple-darwin -fembed-bitcode"
export CPPFLAGS=" -O2 -fno-stack-check -isysroot ${SDKROOT} -miphoneos-version-min=15.0 --target=arm64-apple-darwin -DNO_ASM"
export LDFLAGS=" -isysroot ${SDKROOT} -arch arm64"

export CC_FOR_BUILD="xcrun -sdk macosx clang -arch x86_64"

./configure --enable-cxx --build=x86_64-apple-darwin --host=arm64-apple-darwin --disable-assembly

make -j 12

mv .libs libs_ios_arm64

make clean

cp libs_ios_arm64/libgmp.a ../../libs/libgmp.a
cp libs_ios_arm64/libgmpxx.a ../../libs/libgmpxx.a