// Copyright (c) 2020, the THeBE contributors, as
// shown by the AUTHORS file. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "config_data.h"

#include <algorithm>

#include "system_info.h"
#include "utilities.h"

using namespace cv;
using namespace filesystem;
using namespace std;

ConfigData::ConfigData(const FileStorage& fs) {
    perform_correctness = ReadBool(fs["perform"]["correctness"]);
    perform_average = ReadBool(fs["perform"]["average"]);
    perform_average_ws = ReadBool(fs["perform"]["average_with_steps"]);
    perform_memory = ReadBool(fs["perform"]["memory"]);

    perform_check_std = ReadBool(fs["correctness_tests"]["standard"]);
    perform_check_ws = ReadBool(fs["correctness_tests"]["steps"]);
    perform_check_mem = ReadBool(fs["correctness_tests"]["memory"]);

    output_images = ReadBool(fs["output_images"]);

    average_save_middle_tests = ReadBool(fs["save_middle_tests"]["average"]);
    average_ws_save_middle_tests = ReadBool(fs["save_middle_tests"]["average_with_steps"]);

    average_tests_number = static_cast<int>(fs["tests_number"]["average"]);
    average_ws_tests_number = static_cast<int>(fs["tests_number"]["average_with_steps"]);

    input_txt = "files.txt";
    gnuplot_script_extension = ".gnuplot";
    system_script_extension =
#ifdef THEBE_WINDOWS
        ".bat";
#elif defined(THEBE_LINUX) || defined(THEBE_UNIX) || defined(THEBE_APPLE)
        ".sh";
#endif
    output_image_folder = "output_images";
    middle_folder = "middle_results";
    latex_file = "thebe_results.tex";
    latex_charts = "averageCharts.tex";
    latex_memory_file = "memoryAccesses.tex";
    memory_file = "memory_accesses.txt";

    average_folder = "average_tests";
    average_ws_folder = "average_tests_with_steps";
    memory_folder = "memory_tests";

    output_path = path(fs["paths"]["output"]) / path(GetDatetimeWithoutSpecialChars());
    input_path = path(fs["paths"]["input"]);
    latex_path = output_path / path("latex");

    read(fs["check_datasets"], check_datasets);
    read(fs["average_datasets"], average_datasets);
    read(fs["average_datasets_with_steps"], average_ws_datasets);
    read(fs["memory_datasets"], memory_datasets);

    ReadAlgorithms(fs);

    thebe_os = static_cast<string>(fs["os"]);
}

bool ConfigData::ReadBool(const FileNode& node_list)
{
    bool b = false;
    if (!node_list.empty()) {
        //The entry is found
        string s((string)node_list);
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        istringstream(s) >> boolalpha >> b;
    }
    return b;
}

void ConfigData::ReadAlgorithms(const FileStorage& fs) {

    vector<String> algos;
    read(fs["algorithms"], algos);

    for (const auto& a : algos) {
        string algo_str(a);
        algo_str.erase(std::remove_if(algo_str.begin(), algo_str.end(), std::isblank), algo_str.end());

        AlgorithmNames alg_tmp;
        size_t comma_pos = algo_str.find(",");
        size_t column_pos = algo_str.find(";");

        alg_tmp.test_name = algo_str.substr(0, min(comma_pos, column_pos));

        if (comma_pos != string::npos) {
            alg_tmp.display_name = algo_str.substr(comma_pos + 1, column_pos - (comma_pos + 1));
            if (alg_tmp.display_name == "") {
                alg_tmp.display_name = alg_tmp.test_name;
            }
        }
        else {
            alg_tmp.display_name = alg_tmp.test_name;
        }

        if (column_pos != string::npos) {
            alg_tmp.check_name = algo_str.substr(column_pos + 1, string::npos);
        }

        thin_algorithms.push_back(alg_tmp);
    }
}
