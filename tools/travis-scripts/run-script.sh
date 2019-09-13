#!/bin/bash

# Copyright(c) 2016 - 2019 Federico Bolelli
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met :
# 
# *Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
# 
# * Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and / or other materials provided with the distribution.
# 
# * Neither the name of THeBE nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
