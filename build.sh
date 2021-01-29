#!/bin/bash
read -p "[*] choose build type [1]Debug, [2]Release: " type

typestr=0
if [ $type == 1 ]
then
    typestr="Debug"
else
    typestr="Release"
fi

ROOT_DIR=$(cd $(dirname $0); pwd)
BUILD_PATH=${ROOT_DIR}/build
cd $BUILD_PATH
cmake -DCMAKE_BUILD_TYPE="$typestr" ..
cmake --build . --config $typestr -j8
cmake --install .
