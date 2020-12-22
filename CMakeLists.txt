# Copyright (c) 2020, the THeBE contributors, as
# shown by the AUTHORS file. All rights reserved.
#
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

cmake_minimum_required(VERSION 3.0.0)

set (ProjectName "THeBE")
project (${ProjectName})

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "Install prefix" FORCE)

#Set THeBE variables
set(THEBE_DOWNLOAD_DATASET OFF CACHE BOOL "If flagged the THeBE dataset will be automatically downloaded")
set(THEBE_INPUT_DATASET_PATH "${CMAKE_INSTALL_PREFIX}/input" CACHE PATH "'input' dataset path") 
set(THEBE_OUTPUT_RESULTS_PATH "${CMAKE_INSTALL_PREFIX}/output" CACHE PATH "'output' results path") 
set(THEBE_IS_TRAVIS_CI OFF CACHE BOOL "This variable is used to handle travis-ci parameters, do not change its value")

if(MSVC)
    set(CMAKE_USE_RELATIVE_PATHS ON CACHE INTERNAL "" FORCE)
endif()

# ----------------------------------------------------------------------------
#  Check for c++11 support
# ----------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD_REQUIRED YES) # Force the following command to generate an error when it fails
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS NO)

# ----------------------------------------------------------------------------
#  Build & install layouts
# ----------------------------------------------------------------------------

# Save libs and executables in the same place
set(THEBE_EXECUTABLE_OUTPUT_PATH "${CMAKE_INSTALL_PREFIX}" CACHE PATH "Output directory for applications" )

# ----------------------------------------------------------------------------
# Solution folders
# ----------------------------------------------------------------------------
if(ENABLE_SOLUTION_FOLDERS)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")
endif()

# --------------------
# OpenCV 
# --------------------

set(OpenCV_REQUIRED_PACKAGES "core;imgcodecs;imgproc" CACHE STRING "OpenCV packages required by THeBE are already set")
FIND_PACKAGE(OpenCV REQUIRED ${OpenCV_REQUIRED_PACKAGES})
include_directories( ${OpenCV_INCLUDE_DIRS} )

if(MSVC)
    if("${OpenCV_STATIC}" STREQUAL "ON" OR "${OpenCV_SHARED}" STREQUAL "OFF")
        message(STATUS "Setting static runtime for MSVC")
        foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO) 
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endforeach(flag_var)
    endif()
endif()

# --------------------
# Utility
# --------------------
set(THEBE_OS "${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}" CACHE INTERNAL "")
message(STATUS "OS: ${THEBE_OS}, OpenCV: ${OpenCV_VERSION}")
add_definitions(-DOPENCV_VERSION_MAJOR=${OpenCV_VERSION_MAJOR})

# --------------------
# Project
# --------------------

include_directories("${CMAKE_SOURCE_DIR}/include")

file(GLOB SRCFILES ${SRCFILES} "src/*.cc" "src/*.cpp" "src/*.c")
file(GLOB HFILES ${HFILES} "include/*.h" "include/*.hpp")
file(GLOB INCFILES ${INCFILES} "include/*.inc")

add_definitions(-D_CRT_SECURE_NO_WARNINGS) #To suppress 'fopen' opencv warning/bug  
# Set configuration file    
if(THEBE_IS_TRAVIS_CI)    
    configure_file("${CMAKE_SOURCE_DIR}/doc/config_travis.yaml" "${CMAKE_INSTALL_PREFIX}/config.yaml")
else()
    configure_file("${CMAKE_SOURCE_DIR}/doc/config_base.yaml" "${CMAKE_INSTALL_PREFIX}/config.yaml")
endif()
source_group("Include Files" FILES ${INCFILES})
source_group("Header Files" FILES ${HFILES})
source_group("Source Files" FILES ${SRCFILES})

set (addExe ${SRCFILES} ${HFILES} ${INCFILES} "${CMAKE_INSTALL_PREFIX}/config.yaml")
add_executable(${ProjectName} ${addExe})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${ProjectName}) # Set THeBE as startup group instead of ALL_BUILD
target_link_libraries (${ProjectName}  ${OpenCV_LIBS})

# Creation of output directory
if (NOT EXISTS "${THEBE_OUTPUT_RESULTS_PATH}")
        file(MAKE_DIRECTORY ${THEBE_OUTPUT_RESULTS_PATH})
endif()
    
# Download and unpack dataset if necessary
if(THEBE_DOWNLOAD_DATASET)
    # Creation of input directory
    if (NOT EXISTS "${THEBE_INPUT_DATASET_PATH}")
        file(MAKE_DIRECTORY ${THEBE_INPUT_DATASET_PATH})
    endif()
    
    if (EXISTS "${THEBE_INPUT_DATASET_PATH}/dataset.zip")
        message(STATUS "${THEBE_INPUT_DATASET_PATH}/dataset.zip already exists, download skipped!")
    else()
        message(STATUS "Downloading dataset...")
        file(DOWNLOAD http://imagelab.ing.unimore.it/files/THeBE_dataset.zip "${THEBE_INPUT_DATASET_PATH}/dataset.zip" STATUS status SHOW_PROGRESS)
        list( GET status 0 error_code )
        if( error_code )
            message(ERROR " Cmake is not able to download the THeBE dataset. \nPlease, retry or download it manually and put all unzipped dataset directories in the 'input' path:\n'" ${THEBE_INPUT_DATASET_PATH} "'. \n(The dataset is available at http://imagelab.ing.unimore.it/files/THeBE_dataset.zip).")
            if(EXISTS "${THEBE_INPUT_DATASET_PATH}/dataset.zip")
                file(REMOVE "${THEBE_INPUT_DATASET_PATH}/dataset.zip")
            endif()
        else()
            message(STATUS "Downloading done.")
        endif()
        
        if(EXISTS "${THEBE_INPUT_DATASET_PATH}/dataset.zip")
            message(STATUS "Unpacking (it may take a while)...")
            execute_process(COMMAND ${CMAKE_COMMAND} -E tar xvf "${THEBE_INPUT_DATASET_PATH}/dataset.zip" WORKING_DIRECTORY "${THEBE_INPUT_DATASET_PATH}" OUTPUT_QUIET)
            message(STATUS "Unpacking done.")
        endif()
    endif()
    
endif()
