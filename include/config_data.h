// Copyright(c) 2019 Federico Bolelli, Costantino Grana
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
//
// *Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and / or other materials provided with the distribution.
//
// * Neither the name of THeBE nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef THEBE_CONFIG_DATA_H_
#define THEBE_CONFIG_DATA_H_

#include <opencv2/imgproc.hpp>

#include "file_manager.h"
//#include "system_info.h"
//#include "utilities.h"

struct AlgorithmNames {
    cv::String test_name    = "";  // Name of the algorithm
    cv::String check_name   = "";  // Name of the algorithm to compare results with (required by correctness test);
    cv::String display_name = "";  // Name to be displayed on charts and tables
};

struct ConfigData {

    bool perform_correctness;            // Whether to perform correctness tests or not
    bool perform_average;                // Whether to perform average tests or not
    bool perform_memory;                 // Whether to perform memory tests or not
    bool perform_average_ws;             // Whether to perform average tests with steps or not

    bool perform_check_std;              // Whether to perform correctness on PerformThinning() functions
    bool perform_check_ws;               // Whether to perform correctness on PerformThinningWithSteps() functions
    bool perform_check_mem;              // Whether to perform correctness on PerformThinningMem() functions

    bool output_images;                  // If true, images from tests will be stored
    bool average_save_middle_tests;      // If true, results of each average test run will be stored 
    bool average_ws_save_middle_tests;   // If true, results of each average test with steps run will be stored 

    unsigned average_tests_number;        // Reps of average tests (only the minimum will be considered)
    unsigned average_ws_tests_number;     // Reps of average tests with steps (only the minimum will be considered)

    std::string input_txt;                // File of images list
    std::string gnuplot_script_extension; // Gnuplot scripts extension
    std::string system_script_extension;  // System-based scripts extension
    std::string colors_folder;            // Folder which will store colored images
    std::string middle_folder;            // Folder which will store middle results
    std::string latex_file;               // Latex file which will store textual average results
    std::string latex_memory_file;        // Latex file which will store textual memory results
    std::string latex_charts;             // Latex file which will store report latex code for charts
    std::string memory_file;              // File which will store report textual memory results
    
    std::string average_folder;           // Folder which will store average test results
    std::string average_ws_folder;        // Folder which will store average test with steps results
    std::string memory_folder;            // Folder which will store memory results

    filesystem::path output_path;         // Path on which results are stored
    filesystem::path input_path;          // Path on which input datasets are stored
    filesystem::path latex_path;          // Path on which latex report will be stored

    std::vector<cv::String> check_datasets;       // List of datasets on which check tests will be performed
    std::vector<cv::String> memory_datasets;      // List of datasets on which memory tests will be perform
    std::vector<cv::String> average_datasets;     // Lists of dataset on which average tests will be performed
    std::vector<cv::String> average_ws_datasets;  // Lists of dataset on which average tests whit steps will be performed

    std::vector<AlgorithmNames> thin_algorithms;          // Lists of algorithms specified by the user in the config.yaml
    std::vector<AlgorithmNames> thin_existing_algorithms; // Lists of 'ccl_algorithms' actually existing

	std::vector<AlgorithmNames> thin_mem_algorithms;        // List of algorithms that actually support memory tests
    std::vector<AlgorithmNames> thin_average_algorithms;    // List of algorithms that actually support average tests
    std::vector<AlgorithmNames> thin_average_ws_algorithms; // List of algorithms that actually support average with steps tests
    
    std::string thebe_os;               // Name of the current OS

    ConfigData(const cv::FileStorage& fs);

    bool ReadBool(const cv::FileNode& node_list);

    void ReadAlgorithms(const cv::FileStorage& fs);
};

#endif // !THEBE_CONFIG_DATA_H_
