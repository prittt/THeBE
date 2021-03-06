// Copyright (c) 2020, the THeBE contributors, as
// shown by the AUTHORS file. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cstdint>

#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <opencv2/imgproc.hpp>

#include "config_data.h"
#include "file_manager.h"
#include "thinning_algorithms.h"
#include "memory_tester.h"
#include "performance_evaluator.h"
#include "progress_bar.h"
#include "stream_demultiplexer.h"
#include "system_info.h"
#include "utilities.h"
#include "thebe_tests.h"

using namespace cv;
using namespace filesystem;
using namespace std;

int main()
{
    // Redirect cv exceptions
#if OPENCV_VERSION_MAJOR >= 4
    redirectError(RedirectCvError);
#else
    cvRedirectError(RedirectCvError);
#endif

    // Hide cursor from console
    HideConsoleCursor();

    // To handle filesystem errors
    error_code ec;

    // Create StreamDemultiplexer object in order
    // to print output on both stdout and log file
    string logfile = "log.txt";
    ofstream os(logfile);
    if (os.is_open()) {
        dmux::cout.AddStream(os);
    }

    OutputBox ob_setconf("Setting Configuration Parameters");
    // Read yaml configuration file
    const string config_file = "config.yaml";
    FileStorage fs;
    try {
        fs.open(config_file, FileStorage::READ);
    }
    catch (const cv::Exception&) {
        exit(EXIT_FAILURE);  // Error redirected,
                             // OpenCV redirected function will
                             // print the error on stdout
    }

    if (!fs.isOpened()) {
        ob_setconf.Cerror("Failed to open '" + config_file + "'");
        // EXIT_FAILURE
    }

    // Load configuration data from yaml
    ConfigData cfg(fs);

    // Release FileStorage
    fs.release();

    /*************************************************************************/
    /*  Configuration parameters check                                       */
    /*************************************************************************/

    //// Check if check algo list is correct
    //if (cfg.perform_correctness && (cfg.thin_algorithms.size() != cfg.thin_check_algorithms.size())) {
    //    ob_setconf.Cwarning("check_algorithms list mismatch algorithm list. Correctness test disabled.");
    //    cfg.perform_correctness = false;
    //}

    // Check if all the specified algorithms exist
    for (size_t i = 0; i < cfg.thin_algorithms.size(); ++i) {
        auto& algo_struct = cfg.thin_algorithms[i];
        string algo_name = algo_struct.test_name;
         
        if (!ThinningMapSingleton::Exists(algo_name)) {
            ob_setconf.Cwarning("Unable to find the algorithm '" + algo_name + "'");
        }
        else {
            cfg.thin_existing_algorithms.push_back(algo_struct);
            if (cfg.perform_correctness) {
                string check_algo_name = algo_struct.test_name;
                if (!ThinningMapSingleton::Exists(check_algo_name)) {
                    ob_setconf.Cwarning("Unable to find the check algorithm '" + check_algo_name + "'. Correctness test disabled.");
                    cfg.perform_correctness = false;
                }
            }
        }
    }

    if (cfg.thin_existing_algorithms.size() == 0) {
        ob_setconf.Cerror("There are no valid values in the 'algorithms' list");
    }

    // Check if labeling methods of the specified algorithms exist
    Thinning::img_ = Mat1b(1, 1, static_cast<uchar>(0));
    for (size_t i = 0; i < cfg.thin_existing_algorithms.size(); ++i) {
        auto& algo_struct = cfg.thin_existing_algorithms[i];
        string algo_name = algo_struct.test_name;
        const auto& algorithm = ThinningMapSingleton::GetThinning(algo_name);
        if (cfg.perform_average || (cfg.perform_correctness && cfg.perform_check_std)) {
            try {
                algorithm->PerformThinning();
                cfg.thin_average_algorithms.push_back(algo_struct);

                if (cfg.perform_correctness && cfg.perform_check_std) {
                    string check_algo_name = algo_struct.check_name;
                    const auto& check_algorithm = ThinningMapSingleton::GetThinning(check_algo_name);
                    try {
                        check_algorithm->PerformThinning();
                        // The check name has already been pushed. If it doesn't work properly standard correctness test is disabled (see catch below)
                    }
                    catch (const runtime_error& e) {
                        ob_setconf.Cwarning(check_algo_name + ": " + e.what());
                        ob_setconf.Cwarning("Standard correctness test disabled!");
                        cfg.perform_check_std = false;
                    }
                }
            }
            catch (const runtime_error& e) {
                ob_setconf.Cwarning(algo_name + ": " + e.what());
            }
        }
        if (cfg.perform_average_ws || (cfg.perform_correctness && cfg.perform_check_ws)) {
            try {
                algorithm->PerformThinningWithSteps();
                cfg.thin_average_ws_algorithms.push_back(algo_struct);

                if (cfg.perform_correctness && cfg.perform_check_ws) {
                    string check_algo_name = algo_struct.check_name;
                    const auto& check_algorithm = ThinningMapSingleton::GetThinning(check_algo_name);
                    try {
                        check_algorithm->PerformThinningWithSteps();
                        // The check name has already been pushed. If it doesn't work properly steps correctness test is disabled (see catch below)
                    }
                    catch (const runtime_error& e) {
                        ob_setconf.Cwarning(check_algo_name + ": " + e.what());
                        ob_setconf.Cwarning("Steps correctness test disabled!");
                        cfg.perform_check_ws = false;
                    }
                }
            }
            catch (const runtime_error& e) {
                ob_setconf.Cwarning(algo_name + ": " + e.what());
            }
        }
        if (cfg.perform_memory || (cfg.perform_correctness && cfg.perform_check_mem)) {
            try {
                vector<uint64_t> temp;
                algorithm->PerformThinningMem(temp);
                cfg.thin_mem_algorithms.push_back(algo_struct);

                if (cfg.perform_correctness && cfg.perform_check_mem) {
                    string check_algo_name = algo_struct.check_name;
                    const auto& check_algorithm = ThinningMapSingleton::GetThinning(check_algo_name);
                    try {
                        check_algorithm->PerformThinningMem(temp);
                        // The check name has already been pushed. If it doesn't work properly memory correctness test is disabled (see catch below)
                    }
                    catch (const runtime_error& e) {
                        ob_setconf.Cwarning(check_algo_name + ": " + e.what());
                        ob_setconf.Cwarning("Memory correctness test disabled!");
                        cfg.perform_check_mem = false;
                    }
                }
            }
            catch (const runtime_error& e) {
                ob_setconf.Cwarning(algo_name + ": " + e.what());
            }
        }
    }

    if ((cfg.perform_average || (cfg.perform_correctness && cfg.perform_check_std)) && cfg.thin_average_algorithms.size() == 0) {
        ob_setconf.Cwarning("There are no 'algorithms' with valid 'PerformThinning()' method, related tests will be skipped");
        cfg.perform_average = false;
        cfg.perform_check_std = false;
    }

    if ((cfg.perform_average_ws || (cfg.perform_correctness && cfg.perform_check_ws)) && cfg.thin_average_ws_algorithms.size() == 0) {
        ob_setconf.Cwarning("There are no 'algorithms' with valid 'PerformThinningWithSteps()' method, related tests will be skipped");
        cfg.perform_average_ws = false;
        cfg.perform_check_ws = false;
    }

    if ((cfg.perform_memory || (cfg.perform_correctness && cfg.perform_check_mem)) && cfg.thin_mem_algorithms.size() == 0) {
        ob_setconf.Cwarning("There are no 'algorithms' with valid 'PerformThinningMem()' method, related tests will be skipped");
        cfg.perform_memory = false;
        cfg.perform_check_mem = false;
    }

    if (cfg.perform_average && (cfg.average_tests_number < 1 || cfg.average_tests_number > 999)) {
        ob_setconf.Cwarning("'average test' repetitions cannot be less than 1 or more than 999, skipped");
        cfg.perform_average = false;
    }

    if (cfg.perform_average_ws && (cfg.average_ws_tests_number < 1 || cfg.average_ws_tests_number > 999)) {
        ob_setconf.Cwarning("'average with steps test' repetitions cannot be less than 1 or more than 999, skipped");
        cfg.perform_average_ws = false;
    }

    if ((cfg.perform_correctness) && cfg.check_datasets.size() == 0) {
        ob_setconf.Cwarning("There are no datasets specified for 'correctness test', skipped");
        cfg.perform_correctness = false;
    }

    if ((cfg.perform_average) && cfg.average_datasets.size() == 0) {
        ob_setconf.Cwarning("There are no datasets specified for 'average test', skipped");
        cfg.perform_average = false;
    }

    if ((cfg.perform_average_ws) && cfg.average_ws_datasets.size() == 0) {
        ob_setconf.Cwarning("There are no datasets specified for 'average with steps test', skipped");
        cfg.perform_average_ws = false;
    }

    if ((cfg.perform_memory) && cfg.memory_datasets.size() == 0) {
        ob_setconf.Cwarning("There are no datasets specified for 'memory test', skipped");
        cfg.perform_memory = false;
    }

    if (!cfg.perform_average && !cfg.perform_correctness &&
        !cfg.perform_memory && !cfg.perform_average_ws) {
        ob_setconf.Cerror("There are no tests to perform");
    }

    // Check datasets existence
    {
        std::function<bool(vector<String>&, bool)> CheckDatasetExistence = [&cfg, &ob_setconf, &ec](vector<String>& dataset, bool print_message) -> bool {
            // Check if all the datasets' files.txt exist
            bool exists_one_dataset = false;
            for (auto& x : dataset) {
                path p = cfg.input_path / path(x) / path(cfg.input_txt);
                if (!exists(p, ec)) {
                    if (print_message) {
                        ob_setconf.Cwarning("There is no dataset '" + x + "' (no files.txt available), skipped");
                    }
                }
                else {
                    exists_one_dataset = true;
                }
            }
            return exists_one_dataset;
        };

        vector<String> ds;
        if (cfg.perform_correctness) {
            ds.insert(ds.end(), cfg.check_datasets.begin(), cfg.check_datasets.end());
        }
        if (cfg.perform_memory) {
            ds.insert(ds.end(), cfg.memory_datasets.begin(), cfg.memory_datasets.end());
        }
        if (cfg.perform_average) {
            ds.insert(ds.end(), cfg.average_datasets.begin(), cfg.average_datasets.end());
        }
        if (cfg.perform_average_ws) {
            ds.insert(ds.end(), cfg.average_ws_datasets.begin(), cfg.average_ws_datasets.end());
        }
        std::sort(ds.begin(), ds.end());
        ds.erase(unique(ds.begin(), ds.end()), ds.end());
        CheckDatasetExistence(ds, true); // To check single dataset

        if (cfg.perform_correctness) {
            if (!CheckDatasetExistence(cfg.check_datasets, false)) {
                ob_setconf.Cwarning("There are no valid datasets for 'correctness test', skipped");
                cfg.perform_correctness = false;
            }
        }

        if (cfg.perform_average) {
            if (!CheckDatasetExistence(cfg.average_datasets, false)) {
                ob_setconf.Cwarning("There are no valid datasets for 'average test', skipped");
                cfg.perform_average = false;
            }
        }

        if (cfg.perform_average_ws) {
            if (!CheckDatasetExistence(cfg.average_ws_datasets, false)) {
                ob_setconf.Cwarning("There are no valid datasets for 'average with steps test', skipped");
                cfg.perform_average_ws = false;
            }
        }

        if (cfg.perform_memory) {
            if (!CheckDatasetExistence(cfg.memory_datasets, false)) {
                ob_setconf.Cwarning("There are no valid datasets for 'memory test', skipped");
                cfg.perform_memory = false;
            }
        }
    }

    if (cfg.perform_average || cfg.perform_average_ws || cfg.perform_memory) {
        // Set and create current output directory
        if (!create_directories(cfg.output_path, ec)) {
            ob_setconf.Cerror("Unable to create output directory '" + cfg.output_path.string() + "' - " + ec.message());
        }

        // Create the directory for latex reports
        if (!create_directories(cfg.latex_path, ec)) {
            ob_setconf.Cerror("Unable to create output directory '" + cfg.latex_path.string() + "' - " + ec.message());
        }
    }

    ob_setconf.Cmessage("Setting Configuration Parameters DONE");
    ob_setconf.CloseBox();

    ThebeTests yt(cfg);

    // Correctness test
    if (cfg.perform_correctness) {
        if (cfg.perform_check_std) {
            yt.CheckPerformThinning();
        }

        if (cfg.perform_check_ws) {
           yt.CheckPerformThinningWithSteps();
        }

        if (cfg.perform_check_mem) {
           yt.CheckPerformThinningMem();
        }
    }

    // Average test
    if (cfg.perform_average) {
        yt.AverageTest();
    }

    // Average with steps test
    if (cfg.perform_average_ws) {
        yt.AverageTestWithSteps();
    }

    // Memory test
    if (cfg.perform_memory) {
        yt.MemoryTest();
    }

    // Latex Generator
    if (cfg.perform_average || cfg.perform_average_ws || cfg.perform_memory) {
        yt.LatexGenerator();
    }

    // Copy log file into output folder
    dmux::cout.flush();
    filesystem::copy(path(logfile), cfg.output_path / path(logfile), ec);

    return EXIT_SUCCESS;
}