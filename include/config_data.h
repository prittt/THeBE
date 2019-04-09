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

#include <algorithm>

#include <opencv2/imgproc.hpp>

#include "file_manager.h"
#include "system_info.h"
#include "utilities.h"

using namespace filesystem;

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

    bool perform_check_std;             // Whether to perform correctness on PerformThinning() functions
    bool perform_check_ws;              // Whether to perform correctness on PerformThinningWithSteps() functions
    bool perform_check_mem;             // Whether to perform correctness on PerformThinningMem() functions

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

    path output_path;                     // Path on which results are stored
    path input_path;                      // Path on which input datasets are stored
    path latex_path;                      // Path on which latex report will be stored

    std::vector<cv::String> check_datasets;       // List of datasets on which check tests will be performed
    std::vector<cv::String> memory_datasets;      // List of datasets on which memory tests will be perform
    std::vector<cv::String> average_datasets;     // Lists of dataset on which average tests will be performed
    std::vector<cv::String> average_ws_datasets;  // Lists of dataset on which average tests whit steps will be performed

    std::vector<AlgorithmNames> thin_algorithms;          // Lists of algorithms specified by the user in the config.yaml
    std::vector<AlgorithmNames> thin_existing_algorithms; // Lists of 'ccl_algorithms' actually existing

	std::vector<AlgorithmNames> thin_mem_algorithms;        // List of algorithms that actually support memory tests
    std::vector<AlgorithmNames> thin_average_algorithms;    // List of algorithms that actually support average tests
    std::vector<AlgorithmNames> thin_average_ws_algorithms; // List of algorithms that actually support average with steps tests
    
    std::string yacclab_os;             // Name of the current OS

    ConfigData(const cv::FileStorage& fs) {
        // Flags to customize output format (false by default)
        perform_correctness          = ReadBool(fs["perform"]["correctness"]);
        perform_average              = ReadBool(fs["perform"]["average"]);
        perform_average_ws           = ReadBool(fs["perform"]["average_with_steps"]);
        perform_memory               = ReadBool(fs["perform"]["memory"]);

        perform_check_std            = ReadBool(fs["correctness_tests"]["standard"]);
        perform_check_ws             = ReadBool(fs["correctness_tests"]["steps"]);
        perform_check_mem            = ReadBool(fs["correctness_tests"]["memory"]);

        output_images                = ReadBool(fs["output_images"]);

        average_save_middle_tests    = ReadBool(fs["save_middle_tests"]["average"]);
        average_ws_save_middle_tests = ReadBool(fs["save_middle_tests"]["average_with_steps"]);

        average_tests_number         = static_cast<int>(fs["tests_number"]["average"]);
        average_ws_tests_number      = static_cast<int>(fs["tests_number"]["average_with_steps"]);

        input_txt                    = "files.txt";
        gnuplot_script_extension     = ".gnuplot";
        system_script_extension      =
#ifdef THEBE_WINDOWS
            ".bat";
#elif defined(THEBE_LINUX) || defined(THEBE_UNIX) || defined(THEBE_APPLE)
            ".sh";
#endif
        colors_folder                = "colors";
        middle_folder                = "middle_results";
        latex_file                   = "yacclab_results.tex";
        latex_charts                 = "averageCharts.tex";
        latex_memory_file            = "memoryAccesses.tex";
        memory_file                  = "memory_accesses.txt";

        average_folder               = "average_tests";
        average_ws_folder            = "average_tests_with_steps";
        memory_folder                = "memory_tests";

        output_path                  = path(fs["paths"]["output"]) / path(GetDatetimeWithoutSpecialChars());
        input_path                   = path(fs["paths"]["input"]);
        latex_path                   = output_path / path("latex");

        cv::read(fs["check_datasets"], check_datasets);
        cv::read(fs["average_datasets"], average_datasets);
        cv::read(fs["average_datasets_with_steps"], average_ws_datasets);
        cv::read(fs["memory_datasets"], memory_datasets);
        
        ReadAlgorithms(fs);

        yacclab_os                   = static_cast<std::string>(fs["os"]);
    }

    bool ReadBool(const cv::FileNode& node_list)
    {
        bool b = false;
        if (!node_list.empty()) {
            //The entry is found
            std::string s((std::string)node_list);
            std::transform(s.begin(), s.end(), s.begin(), ::tolower);
            std::istringstream(s) >> std::boolalpha >> b;
        }
        return b;
    }

    void ReadAlgorithms(const cv::FileStorage& fs) {
        
        std::vector<cv::String> algos;
        cv::read(fs["algorithms"], algos);

        for (const auto& a : algos) {
            std::string algo_str(a);
            algo_str.erase(std::remove_if(algo_str.begin(), algo_str.end(), ::isblank), algo_str.end());
            
            AlgorithmNames alg_tmp;
            size_t comma_pos = algo_str.find(",");
            size_t column_pos = algo_str.find(";");

            alg_tmp.test_name = algo_str.substr(0, std::min(comma_pos, column_pos));
            
            if (comma_pos != std::string::npos) {
                alg_tmp.display_name = algo_str.substr(comma_pos + 1, column_pos - (comma_pos + 1));
                if (alg_tmp.display_name == "") {
                    alg_tmp.display_name = alg_tmp.test_name;
                }
            }
            else {
                alg_tmp.display_name = alg_tmp.test_name;
            }

            if (column_pos != std::string::npos) {
                alg_tmp.check_name = algo_str.substr(column_pos + 1, std::string::npos);
            }

            thin_algorithms.push_back(alg_tmp);
        }
    }

};

#endif // !THEBE_CONFIG_DATA_H_
