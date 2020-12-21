#!/bin/bash

# Copyright (c) 2020, the THeBE contributors, as
# shown by the AUTHORS file. All rights reserved.
#
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

# exit this script if any commmand fails
set -e

function build_linux(){

   echo -e "############################################### Build and Test ###############################################"

   echo -e "\n\n------------------------------------------> THeBE configuration" 
   mkdir bin
   # The download of the complete THeBE dataset is disable in order to reduce the cmake configure time in travis-ci virtual machine
   cmake -D CMAKE_C_FLAGS=-m64 -D CMAKE_CXX_FLAGS=-m64 -D CMAKE_BUILD_TYPE=Release -D CMAKE_PREFIX_PATH=./opencv-3.1.0/build/ -D THEBE_DOWNLOAD_DATASET=OFF -D THEBE_IS_TRAVIS_CI=ON -G Unix\ Makefiles -Bbin -Hbin/.. 

   cd bin
   
   if [ ! -f config.yaml ]; then
      echo "Configuration file (config.yaml) was not properly generated by CMake, pull request failed"
	  exit 1
   fi
   cat config.yaml
   echo -e "------------------------------------------> DONE!"
   
   # Download of a reduced version of the THeBE dataset, which is part of the reduced YACCLAB dataset
   echo -e "\n\n------------------------------------------> Download of THeBE reduced dataset" 
   #curl -L --progress-bar http://imagelab.ing.unimore.it/files/YACCLAB_dataset.zip > dataset.zip
   wget http://imagelab.ing.unimore.it/files/YACCLAB_dataset_reduced.zip -O dataset.zip
   unzip -qq dataset.zip
   rm dataset.zip  
   echo -e "------------------------------------------> DONE!"
   
   echo -e "\n\n------------------------------------------> Build and Test THeBE" 
   #rm config.yaml
   #cp ../doc/config_travis.yaml .
   #mv config_travis.yaml config.yaml
 
   make 
   ./THeBE
   echo -e "------------------------------------------> DONE!"
}

function build_mac(){

   echo -e "############################################### Build and Test ###############################################"
 
   echo -e "\n\n------------------------------------------> THeBE configuration" 
   mkdir bin
   cmake -D CMAKE_C_FLAGS=-m64 -D CMAKE_CXX_FLAGS=-m64 -D CMAKE_BUILD_TYPE=Release -D CMAKE_PREFIX_PATH=./opencv-3.1.0/build/ -D THEBE_DOWNLOAD_DATASET=OFF -D THEBE_IS_TRAVIS_CI=ON -G Xcode -Bbin -Hbin/.. 

   cd bin
   
   if [ ! -f config.yaml ]; then
      echo "Configuration file (config.yaml) was not properly generated by CMake, pull request failed"
	  exit 1
   fi
   cat config.yaml
   echo -e "------------------------------------------> DONE!"

   # Download of a reduced version of the THeBE dataset, which is part of the reduced YACCLAB dataset
   echo -e "\n\n------------------------------------------> Download of THeBE reduced dataset" 
   #curl -L --progress-bar http://imagelab.ing.unimore.it/files/YACCLAB_dataset.zip > dataset.zip
   wget http://imagelab.ing.unimore.it/files/YACCLAB_dataset_reduced.zip -O dataset.zip
   unzip -qq dataset.zip
   rm dataset.zip  
   echo -e "------------------------------------------> DONE!"
   
   echo -e "\n\n------------------------------------------> Build and Test THeBE" 
   #rm config.yaml
   #cp ../doc/config_travis.yaml .
   #mv config_travis.yaml config.yaml
 
   # xcodebuild -project YACCLAB.xcodeproj -target YACCLAB -configuration Release > 
   xcodebuild -project THeBE.xcodeproj -target THeBE -configuration Release
   ./Release/THeBE
   echo -e "------------------------------------------> DONE!"
}

function pass(){
	echo "pass"
}

function run_pull_request(){

    # linux
    if [ "$BUILD_TARGET" == "linux" ]; then
        build_linux
    fi

    if [ "$BUILD_TARGET" == "mac" ]; then
        build_mac
    fi
}

# build pull request
#if [ "$TRAVIS_PULL_REQUEST" != "false" ]; then
	run_pull_request
#fi
