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

#include "thebe_tests.h"

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <cstdint>

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include "thinning_algorithms.h"
#include "latex_generator.h"
#include "memory_tester.h"
#include "utilities.h"

using namespace std;
using namespace cv;

// Load a list of image names from a specified file (files_path) and store them into a vector of
// pairs (filenames). Each pairs contains the name of the file (first) and a bool (second)
// representing file state.
bool ThebeTests::LoadFileList(vector<pair<string, bool>>& filenames, const path& files_path)
{
    // Open files_path (files.txt)
    ifstream is(files_path.string());
    if (!is.is_open()) {
        return false;
    }

    string cur_filename;
    while (getline(is, cur_filename)) {
        // To delete possible carriage return in the file name
        // (especially designed for windows file newline format)
        RemoveCharacter(cur_filename, '\r');
        filenames.push_back(make_pair(cur_filename, true));
    }

    is.close();
    return true;
}

// Check if all the files in a list of pair (filename, state) exists and set the state of every file
// opportunely. The function returns true if all the files exist, false otherwise.
bool ThebeTests::CheckFileList(const path& base_path, vector<pair<string, bool>>& filenames)
{
    bool ret = true;
    for (size_t i = 0; i < filenames.size(); ++i) {
        error_code ec;
        filenames[i].second = filesystem::exists(base_path / path(filenames[i].first), ec);
        if (!filenames[i].second) {
            ret = false;
        }
    }
    return ret;
}

// This function take a Mat1d of results and save it in the specified output-stream
bool ThebeTests::SaveBroadOutputResults(map<String, Mat1d>& results, const string& o_filename, const Mat1i& labels, const vector<pair<string, bool>>& filenames, const std::vector<cv::String>& ccl_algorithms)
{
    ofstream os(o_filename);
    if (!os.is_open()) {
        return false;
    }

    // To set heading file format
    os << "#" << '\t';
    for (const auto& algo_name : ccl_algorithms) {

        // Calculate the max of the columns to find unused steps
        Mat1d results_reduced(1, results.at(algo_name).cols);
#if OPENCV_VERSION_MAJOR >= 4
        cv::reduce(results.at(algo_name), results_reduced, 0, REDUCE_MAX);
#else
        cv::reduce(results.at(algo_name), results_reduced, 0, CV_REDUCE_MAX);
#endif

        for (int step_number = 0; step_number != StepType::ST_SIZE; ++step_number) {
            StepType step = static_cast<StepType>(step_number);
            double column_value(results_reduced(0, step_number));
            if (column_value != numeric_limits<double>::max()) {
                os << algo_name + "_" << Step(step) << '\t';
            }
        }
    }
    os << '\n';

    for (unsigned files = 0; files < filenames.size(); ++files) {
        if (filenames[files].second) {
            os << filenames[files].first << '\t';
            unsigned i = 0;
            for (const auto& algo_name : ccl_algorithms) {
                for (int step_number = 0; step_number != StepType::ST_SIZE; ++step_number) {
                    if (results.at(algo_name)(files, step_number) != numeric_limits<double>::max()) {
                        os << results.at(algo_name)(files, step_number) << '\t';
                    }
                    else {
                        // Step not held, skipped
                        //os << 0 << '\t';
                    }
                }
                ++i;
            }
            os << '\n';
        }
    }

    return true;
}

bool ThebeTests::SaveBroadOutputResults(const Mat1d& results, const string& o_filename, const Mat1i& labels, const vector<pair<string, bool>>& filenames, const std::vector<cv::String>& ccl_algorithms)
{
    ofstream os(o_filename);
    if (!os.is_open()) {
        return false;
    }

    // To set heading file format
    os << "#";
    for (const auto& algo_name : ccl_algorithms) {
        os << '\t' << algo_name;
    }
    os << '\n';
    // To set heading file format

    for (unsigned files = 0; files < filenames.size(); ++files) {
        if (filenames[files].second) {
            os << filenames[files].first << '\t';
            for (unsigned i = 0; i < ccl_algorithms.size(); ++i) {
                os << results(files, i) << '\t';
            }
            os << '\n';
        }
    }
    return true;
}

// To calculate average times and write it on the specified file
void ThebeTests::SaveAverageWithStepsResults(const string& os_name, const String& dataset_name, bool rounded)
{
    ofstream os(os_name);
    if (!os.is_open()) {
        dmux::cout << "Unable to save average results" << '\n';
        return;
    }

    // Write heading string in output stream
    os << "#Algorithm" << '\t';
    for (int step_number = 0; step_number != StepType::ST_SIZE; ++step_number) {
        StepType step = static_cast<StepType>(step_number);
        os << Step(step) << '\t';
    }
    os << "Total" << '\n';

    const auto& results = average_ws_results_.at(dataset_name);

    for (int r = 0; r < results.rows; ++r) {
        const auto& algo_name = cfg_.ccl_average_ws_algorithms[r];
        double cumulative_sum{ 0.0 };

        os << DoubleEscapeUnderscore(string(algo_name)) << '\t';

        for (int c = 0; c < results.cols; ++c) {
            if (rounded) {
                cumulative_sum += floor(results(r, c) * 100.00 + 0.5) / 100.00;
                os << std::fixed << std::setprecision(2) << results(r, c) << '\t';
            }
            else {
                cumulative_sum += results(r, c);
                os << std::fixed << std::setprecision(8) << results(r, c) << '\t';
            }
        }
        // Write cumulative_sum as total
        if (rounded) {
            os << std::fixed << std::setprecision(2) << cumulative_sum;
        }
        else {
            os << std::fixed << std::setprecision(8) << cumulative_sum;
        }
        os << '\n';
    }
    os.close();
}

void ThebeTests::AverageTest()
{
    OutputBox ob("Average Test");

    string complete_results_suffix = "_results.txt",
        middle_results_suffix = "_run",
        average_results_suffix = "_average.txt";

    // Initialize results container
    average_results_ = cv::Mat1d(static_cast<unsigned>(cfg_.average_datasets.size()), static_cast<unsigned>(cfg_.ccl_average_algorithms.size()), std::numeric_limits<double>::max());

    for (unsigned d = 0; d < cfg_.average_datasets.size(); ++d) { // For every dataset in the average list

        String dataset_name(cfg_.average_datasets[d]),
            output_average_results = dataset_name + average_results_suffix,
            output_graph = dataset_name + kTerminalExtension,
            output_graph_bw = dataset_name + "_bw" + kTerminalExtension;

        path dataset_path(cfg_.input_path / path(dataset_name)),
            is_path = dataset_path / path(cfg_.input_txt), // files.txt path
            current_output_path(cfg_.output_path / path(cfg_.average_folder) / path(dataset_name)),
            output_broad_path = current_output_path / path(dataset_name + complete_results_suffix),
            output_colored_images_path = current_output_path / path(cfg_.colors_folder),
            output_middle_results_path = current_output_path / path(cfg_.middle_folder),
            average_os_path = current_output_path / path(output_average_results);

        if (!create_directories(current_output_path)) {
            ob.Cwarning("Unable to find/create the output path '" + current_output_path.string() + "', skipped", dataset_name);
            continue;
        }

        if (cfg_.output_images) {
            if (!create_directories(output_colored_images_path)) {
                ob.Cwarning("Unable to find/create the output path '" + output_colored_images_path.string() + "', colored images won't be saved");
            }
        }

        // For AVERAGE
        ofstream average_os(average_os_path.string());
        if (!average_os.is_open()) {
            ob.Cwarning("Unable to open '" + average_os_path.string() + "', skipped", dataset_name);
            continue;
        }

        // To save list of filename on which CLLAlgorithms must be tested
        vector<pair<string, bool>> filenames;  // first: filename, second: state of filename (find or not)
        if (!LoadFileList(filenames, is_path)) {
            ob.Cwarning("Unable to open '" + is_path.string() + "', skipped", dataset_name);
            continue;
        }

        // Number of files
        int filenames_size = static_cast<unsigned>(filenames.size());

        // To save middle/min and average results;
        Mat1d min_res(filenames_size, static_cast<unsigned>(cfg_.ccl_average_algorithms.size()), numeric_limits<double>::max());
        Mat1d current_res(filenames_size, static_cast<unsigned>(cfg_.ccl_average_algorithms.size()), numeric_limits<double>::max());
        Mat1i labels(filenames_size, static_cast<unsigned>(cfg_.ccl_average_algorithms.size()), 0);
        vector<pair<double, uint16_t>> supp_average(cfg_.ccl_average_algorithms.size(), make_pair(0.0, 0));

        // Start output message box
        ob.StartRepeatedBox(dataset_name, filenames_size, cfg_.average_tests_number);

        if (cfg_.average_ws_save_middle_tests) {
            if (!create_directories(output_middle_results_path)) {
                ob.Cwarning("Unable to find/create the output path '" + output_middle_results_path.string() + "', middle results won't be saved");
            }
        }

        map<String, unsigned> algo_pos;
        for (size_t i = 0; i < cfg_.ccl_average_algorithms.size(); ++i)
            algo_pos[cfg_.ccl_average_algorithms[i]] = static_cast<unsigned>(i);
        auto shuffled_ccl_average_algorithms = cfg_.ccl_average_algorithms;

        // Test is executed n_test times
        for (unsigned test = 0; test < cfg_.average_tests_number; ++test) {
            // For every file in list
            for (unsigned file = 0; file < filenames.size(); ++file) {
                // Display output message box
                ob.UpdateRepeatedBox(file);

                string filename = filenames[file].first;
                path filename_path = dataset_path / path(filename);

                // Read and load image
                if (!GetBinaryImage(filename_path, Thinning::img_)) {
                    ob.Cwarning("Unable to open '" + filename + "', skipped");
                    continue;
                }

                random_shuffle(begin(shuffled_ccl_average_algorithms), end(shuffled_ccl_average_algorithms));

                // For all the Algorithms in the array
                for (const auto& algo_name : shuffled_ccl_average_algorithms) {
                    Thinning *algorithm = ThinningMapSingleton::GetThinning(algo_name);
                    unsigned i = algo_pos[algo_name];

                    try {
                        // Perform current algorithm on current image and save result.
                        algorithm->perf_.start();
                        algorithm->PerformThinning();
                        algorithm->perf_.stop();

                    }
                    catch (const exception& e) {
                        algorithm->FreeThinningData();
                        ob.Cerror("Something wrong with " + algo_name + ": " + e.what()); // You should check your algorithms' implementation before performing THeBE tests  
                    }

                    // Save time results
                    current_res(file, i) = algorithm->perf_.last();
                    if (algorithm->perf_.last() < min_res(file, i)) {
                        min_res(file, i) = algorithm->perf_.last();
                    }

					if (cfg_.output_images) {
						String colored_image = (output_colored_images_path / path(filename + "_" + algo_name + ".png")).string();
						imwrite(colored_image, algorithm->img_out_);
					}

                    algorithm->FreeThinningData();
                } // END ALGORITHMS FOR
            } // END FILES FOR.
            ob.StopRepeatedBox(false);

            // Save middle results if necessary (flag 'average_save_middle_tests' enabled)
            if (cfg_.average_save_middle_tests) {
                string output_middle_results_file = (output_middle_results_path / path(dataset_name + middle_results_suffix + "_" + to_string(test) + ".txt")).string();
                if (!SaveBroadOutputResults(current_res, output_middle_results_file, labels, filenames, cfg_.ccl_average_algorithms)) {
                    ob.Cwarning("Unable to save middle results for 'average test'");
                }
            }
        } // END TEST FOR

        // To write in a file min results
        if (!SaveBroadOutputResults(min_res, output_broad_path.string(), labels, filenames, cfg_.ccl_average_algorithms)) {
            ob.Cwarning("Unable to save min results for 'average test'");
        }

        // To calculate average times and write it on the specified file
        for (int r = 0; r < min_res.rows; ++r) {
            for (int c = 0; c < min_res.cols; ++c) {
                if (min_res(r, c) != numeric_limits<double>::max()) {
                    supp_average[c].first += min_res(r, c);
                    supp_average[c].second++;
                }
            }
        }

        average_os << "#Algorithm" << '\t' << "Average" << '\t' << "Round Average for Graphs" << '\n';
        for (unsigned i = 0; i < cfg_.ccl_average_algorithms.size(); ++i) {
            // For all the Algorithms in the array
            const auto& algo_name = cfg_.ccl_average_algorithms[i];

            // Gnuplot requires double-escaped name in presence of underscores
            {
                string algo_name_double_escaped = algo_name;
                std::size_t found = algo_name_double_escaped.find_first_of("_");
                while (found != std::string::npos) {
                    algo_name_double_escaped.insert(found, "\\\\");
                    found = algo_name_double_escaped.find_first_of("_", found + 3);
                }
                average_os << algo_name_double_escaped << '\t';
            }

            // Save all the results
            average_results_(d, i) = supp_average[i].first / supp_average[i].second;
            average_os << std::fixed << std::setprecision(8) << supp_average[i].first / supp_average[i].second << '\t';
            // TODO numberOfDecimalDigitToDisplayInGraph in configuration file ?
            average_os << std::fixed << std::setprecision(2) << supp_average[i].first / supp_average[i].second << '\n';
        }

        { // GNUPLOT SCRIPT
            SystemInfo s_info(cfg_);
            string compiler_name(s_info.compiler_name());
            string compiler_version(s_info.compiler_version());
            //replace the . with _ for compiler strings
            std::replace(compiler_version.begin(), compiler_version.end(), '.', '_');

            path script_os_path = current_output_path / path(dataset_name + cfg_.gnuplot_script_extension);

            ofstream script_os(script_os_path.string());
            if (!script_os.is_open()) {
                ob.Cwarning("Unable to create " + script_os_path.string());
            }

            script_os << "# This is a gnuplot (http://www.gnuplot.info/) script!" << '\n';
            script_os << "# comment fifth line, open gnuplot's teminal, move to script's path and launch 'load " << dataset_name + cfg_.gnuplot_script_extension << "' if you want to run it" << '\n' << '\n';

            script_os << "reset" << '\n';
            script_os << "cd '" << current_output_path.string() << "\'" << '\n';
            script_os << "set grid ytic" << '\n';
            script_os << "set grid" << '\n' << '\n';

            script_os << "# " << dataset_name << "(COLORS)" << '\n';
            script_os << "set output \"" + output_graph + "\"" << '\n';

            script_os << "set title " << GetGnuplotTitle(cfg_) << '\n' << '\n';

            script_os << "# " << kTerminal << " colors" << '\n';
            script_os << "set terminal " << kTerminal << " enhanced color font ',15'" << '\n' << '\n';

            script_os << "# Graph style" << '\n';
            script_os << "set style data histogram" << '\n';
            script_os << "set style histogram cluster gap 1" << '\n';
            script_os << "set style fill solid 0.25 border -1" << '\n';
            script_os << "set boxwidth 0.9" << '\n' << '\n';

            script_os << "# Get stats to set labels" << '\n';
            script_os << "stats \"" << output_average_results << "\" using 3 nooutput" << '\n';
            script_os << "ymax = STATS_max + (STATS_max/100)*10" << '\n';
            script_os << "xw = 0" << '\n';
            script_os << "yw = (ymax)/22.0" << '\n' << '\n';

            script_os << "# Axes labels" << '\n';
            script_os << "set xtic rotate by -45 scale 0" << '\n';
            script_os << "set ylabel \"Execution Time [ms]\"" << '\n' << '\n';

            script_os << "# Axes range" << '\n';
            script_os << "set yrange[0:ymax]" << '\n';
            script_os << "set xrange[*:*]" << '\n' << '\n';

            script_os << "# Legend" << '\n';
            script_os << "set key off" << '\n' << '\n';

            script_os << "# Plot" << '\n';
            script_os << "plot \\" << '\n';

            script_os << "'" + output_average_results + "' using 3:xtic(1), '" << output_average_results << "' using ($0 - xw) : ($3 + yw) : (stringcolumn(3)) with labels" << '\n' << '\n';

            script_os << "# Replot in latex folder" << '\n';
            script_os << "set title \"\"" << '\n' << '\n';
            script_os << "set output \'" << (cfg_.latex_path / path(compiler_name + compiler_version + "_" + output_graph)).string() << "\'" << '\n';
            script_os << "replot" << '\n' << '\n';

            script_os << "# " << dataset_name << "(BLACK AND WHITE)" << '\n';
            script_os << "set output \"" + output_graph_bw + "\"" << '\n';

            script_os << "set title " << GetGnuplotTitle(cfg_) << '\n' << '\n';

            script_os << "# " << kTerminal << " black and white" << '\n';
            script_os << "set terminal " << kTerminal << " enhanced monochrome dashed font ',15'" << '\n' << '\n';

            script_os << "replot" << '\n' << '\n';

            script_os << "exit gnuplot" << '\n';

            average_os.close();
            script_os.close();
        } // GNUPLOT SCRIPT

        if (0 != std::system(("gnuplot \"" + (current_output_path / path(dataset_name + cfg_.gnuplot_script_extension)).string() + "\" 2> gnuplot_errors.txt").c_str())) {
            ob.Cwarning("Unable to run gnuplot script");
        }
        ob.CloseBox();
    } // END DATASET FOR
}

void ThebeTests::AverageTestWithSteps()
{
    // Initialize output message box
    OutputBox ob("Average Test With Steps");

    string complete_results_suffix = "_results.txt",
        middle_results_suffix = "_run",
        average_results_suffix = "_average.txt",
        average_results_rounded_suffix = "_average_rounded.txt";

    for (unsigned d = 0; d < cfg_.average_ws_datasets.size(); ++d) { // For every dataset in the average list

        String dataset_name(cfg_.average_ws_datasets[d]),
            output_average_results = dataset_name + average_results_suffix,
            output_average_results_rounded = dataset_name + average_results_rounded_suffix,
            output_graph = dataset_name + kTerminalExtension,
            output_graph_bw = dataset_name + "_bw" + kTerminalExtension;

        path dataset_path(cfg_.input_path / path(dataset_name)),
            is_path = dataset_path / path(cfg_.input_txt), // files.txt path
            current_output_path(cfg_.output_path / path(cfg_.average_ws_folder) / path(dataset_name)),
            output_broad_path = current_output_path / path(dataset_name + complete_results_suffix),
            output_middle_results_path = current_output_path / path(cfg_.middle_folder),
            average_os_path = current_output_path / path(output_average_results),
            average_rounded_os_path = current_output_path / path(output_average_results_rounded);

        if (!create_directories(current_output_path)) {
            ob.Cwarning("Unable to find/create the output path '" + current_output_path.string() + "', skipped", dataset_name);
            continue;
        }

        // Initialize results container
        average_ws_results_[dataset_name] = Mat1d(static_cast<unsigned>(cfg_.ccl_average_ws_algorithms.size()), StepType::ST_SIZE, numeric_limits<double>::max());

        // To save list of filename on which CLLAlgorithms must be tested
        vector<pair<string, bool>> filenames;  // first: filename, second: state of filename (find or not)
        if (!LoadFileList(filenames, is_path)) {
            ob.Cwarning("Unable to open '" + is_path.string() + "'", dataset_name);
            continue;
        }

        // Number of files
        unsigned filenames_size = static_cast<unsigned>(filenames.size());

        // To save middle/min and average results;
        map<String, Mat1d> current_res;
        map<String, Mat1d> min_res;
        Mat1i labels(filenames_size, static_cast<unsigned>(cfg_.ccl_average_ws_algorithms.size()), 0);

        for (const auto& algo_name : cfg_.ccl_average_ws_algorithms) {
            current_res[algo_name] = Mat1d(filenames_size, StepType::ST_SIZE, numeric_limits<double>::max());
            min_res[algo_name] = Mat1d(filenames_size, StepType::ST_SIZE, numeric_limits<double>::max());
        }

        // Start output message box
        ob.StartRepeatedBox(dataset_name, filenames_size, cfg_.average_ws_tests_number);

        if (cfg_.average_ws_save_middle_tests) {
            if (!create_directories(output_middle_results_path)) {
                ob.Cwarning("Unable to find/create the output path '" + output_middle_results_path.string() + "', middle results won't be saved");
            }
        }

        map<String, unsigned> algo_pos;
        for (size_t i = 0; i < cfg_.ccl_average_ws_algorithms.size(); ++i)
            algo_pos[cfg_.ccl_average_ws_algorithms[i]] = static_cast<unsigned>(i);
        auto shuffled_ccl_average_ws_algorithms = cfg_.ccl_average_ws_algorithms;

        // Test is executed n_test times
        for (unsigned test = 0; test < cfg_.average_ws_tests_number; ++test) {
            // For every file in list
            for (unsigned file = 0; file < filenames.size(); ++file) {
                // Display output message box
                ob.UpdateRepeatedBox(file);

                string filename = filenames[file].first;
                path filename_path = dataset_path / path(filename);

                // Read and load image
                if (!GetBinaryImage(filename_path, Thinning::img_)) {
                    ob.Cwarning("Unable to open '" + filename + "'");
                    continue;
                }

                random_shuffle(begin(shuffled_ccl_average_ws_algorithms), end(shuffled_ccl_average_ws_algorithms));

                // For all the Algorithms in the array
                for (const auto& algo_name : shuffled_ccl_average_ws_algorithms) {
                    Thinning *algorithm = ThinningMapSingleton::GetThinning(algo_name);
                    unsigned i = algo_pos[algo_name];

                    try {
                        // Perform current algorithm on current image and save result.
                        algorithm->PerformThinningWithSteps();
                    }
                    catch (const exception& e) {
                        algorithm->FreeThinningData();
                        ob.Cerror("Something wrong with " + algo_name + ": " + e.what()); // You should check your algorithms' implementation before performing THeBE tests  
                    }

                    // Save time results of all the steps
                    for (int step_number = 0; step_number != StepType::ST_SIZE; ++step_number) {
                        string step = Step(static_cast<StepType>(step_number));

                        // Find if the current algorithm has the current step
                        if (algorithm->perf_.find(step)) {
                            current_res[algo_name](file, step_number) = algorithm->perf_.get(step);
                            if (algorithm->perf_.get(step) < min_res[algo_name](file, step_number)) {
                                min_res[algo_name](file, step_number) = algorithm->perf_.get(step);
                            }
                        }
                    }
                    algorithm->FreeThinningData();
                } // END ALGORITHMS FOR
            } // END FILES FOR.
            ob.StopRepeatedBox(false);

            // Save middle results if necessary (flag 'average_save_middle_tests' enabled)
            if (cfg_.average_ws_save_middle_tests) {
                string output_middle_results_file = (output_middle_results_path / path(dataset_name + middle_results_suffix + "_" + to_string(test) + ".txt")).string();
                SaveBroadOutputResults(current_res, output_middle_results_file, labels, filenames, cfg_.ccl_average_ws_algorithms);
            }
        }// END TESTS FOR

        // To write in a file min results
        SaveBroadOutputResults(min_res, output_broad_path.string(), labels, filenames, cfg_.ccl_average_ws_algorithms);

        // If true the i-th step is used by all the algorithms
        vector<bool> steps_presence(StepType::ST_SIZE, false);

        // To calculate average times and write it on the specified file
        for (unsigned a = 0; a < cfg_.ccl_average_ws_algorithms.size(); ++a) {
            const auto& algo_name(cfg_.ccl_average_ws_algorithms[a]);
            vector<pair<double, uint16_t>> supp_average(StepType::ST_SIZE, make_pair(0.0, 0));

            for (int r = 0; r < min_res.at(algo_name).rows; ++r) {
                for (int c = 0; c < min_res.at(algo_name).cols; ++c) {
                    if (min_res.at(algo_name)(r, c) != numeric_limits<double>::max()) {
                        supp_average[c].first += min_res.at(algo_name)(r, c);
                        supp_average[c].second++;
                    }
                }
            }

            // Matrix reduce done, save the results into the average file
            for (int step_number = 0; step_number != StepType::ST_SIZE; ++step_number) {
                double avg{ 0.0 };
                if (supp_average[step_number].first > 0.0 && supp_average[step_number].second > 0) {
                    steps_presence[step_number] = true;
                    avg = supp_average[step_number].first / supp_average[step_number].second;
                }
                else {
                    // The current step is not threated by the current algorithm, write 0
                }
                average_ws_results_[dataset_name](a, step_number) = avg;
            }
        }

        // Write the results stored in average_ws_results_ in file
        SaveAverageWithStepsResults(average_os_path.string(), dataset_name, false);
        SaveAverageWithStepsResults(average_rounded_os_path.string(), dataset_name, true);

        // GNUPLOT SCRIPT
        {
            SystemInfo s_info(cfg_);
            string compiler_name(s_info.compiler_name());
            string compiler_version(s_info.compiler_version());
            //replace the . with _ for compiler strings
            std::replace(compiler_version.begin(), compiler_version.end(), '.', '_');

            path script_os_path = current_output_path / path(dataset_name + cfg_.gnuplot_script_extension);

            ofstream script_os(script_os_path.string());
            if (!script_os.is_open()) {
                ob.Cwarning("Unable to create " + script_os_path.string());
            }

            script_os << "# This is a gnuplot (http://www.gnuplot.info/) script!" << '\n';
            script_os << "# comment fifth line, open gnuplot's teminal, move to script's path and launch 'load " << dataset_name + cfg_.gnuplot_script_extension << "' if you want to run it" << '\n' << '\n';

            script_os << "reset" << '\n';
            script_os << "cd '" << current_output_path.string() << "\'" << '\n';
            script_os << "set grid ytic" << '\n';
            script_os << "set grid" << '\n' << '\n';

            script_os << "# " << dataset_name << "(COLORS)" << '\n';
            script_os << "set output \"" + output_graph + "\"" << '\n';

            script_os << "set title " << GetGnuplotTitle(cfg_) << '\n' << '\n';

            script_os << "# " << kTerminal << " colors" << '\n';
            script_os << "set terminal " << kTerminal << " enhanced color font ',15'" << '\n' << '\n';

            script_os << "# Graph style" << '\n';
            script_os << "set style data histogram" << '\n';
            script_os << "set style histogram cluster gap 1" << '\n';
            script_os << "set style histogram rowstacked" << '\n';
            script_os << "set style fill solid 0.25 border -1" << '\n';
            script_os << "set boxwidth 0.6" << '\n' << '\n';

            script_os << "# Get stats to set labels" << '\n';
            script_os << "stats \"" << output_average_results_rounded << "\" using 6 nooutput" << '\n';
            script_os << "ymax = STATS_max + (STATS_max/100)*10" << '\n';
            script_os << "xw = 0" << '\n';
            script_os << "yw = (ymax)/22.0" << '\n' << '\n';

            script_os << "# Axes labels" << '\n';
            script_os << "set xtic rotate by -45 scale 0" << '\n';
            script_os << "set ylabel \"Execution Time [ms]\"" << '\n' << '\n';

            script_os << "# Axes range" << '\n';
            script_os << "set yrange[0:ymax]" << '\n';
            script_os << "set xrange[*:*]" << '\n' << '\n';

            script_os << "# Legend" << '\n';
            script_os << "set key outside left font ', 8'" << '\n' << '\n';

            script_os << "# Plot" << '\n';
            script_os << "plot \\" << '\n';

            script_os << "'" + output_average_results_rounded + "' using 2:xtic(1) title '" << Step(static_cast<StepType>(0)) << "', \\" << '\n';
            unsigned i = 3;
            // Start from the second step
            for (int step_number = 1; step_number != StepType::ST_SIZE; ++step_number, ++i) {
                StepType step = static_cast<StepType>(step_number);
                // Add the step only if it used by almost an algorithm
                if (steps_presence[step_number]) {
                    script_os << "'' u " << i << " title '" << Step(step) << "', \\" << '\n';
                }
            }
            const unsigned start_n = 2;
            i = 2;
            for (; i <= StepType::ST_SIZE + 1; ++i) {
                script_os << "'' u ($0) : ((";
                for (unsigned j = i; j >= start_n; --j) {
                    script_os << "$" << j;
                    if (j > start_n) {
                        script_os << "+";
                    }
                }
                script_os << ") - ($" << i << "/2)) : ($" << i << "!=0.0 ? sprintf(\"%4.2f\",$" << i << "):'') w labels font 'Tahoma, 8' title '', \\" << '\n';
            }
            script_os << "'' u ($0) : ($" << i << " + yw) : ($" << i << "!=0.0 ? sprintf(\"%4.2f\",$" << i << "):'') w labels font 'Tahoma' title '', \\" << '\n';

            script_os << "# Replot in latex folder" << '\n';
            script_os << "set title \"\"" << '\n' << '\n';
            script_os << "set output \'" << (cfg_.latex_path / path(compiler_name + compiler_version + "_with_steps_" + output_graph)).string() << "\'" << '\n';
            script_os << "replot" << '\n' << '\n';

            script_os << "# " << dataset_name << "(BLACK AND WHITE)" << '\n';
            script_os << "set output \"" + output_graph_bw + "\"" << '\n';

            script_os << "set title " << GetGnuplotTitle(cfg_) << '\n' << '\n';

            script_os << "# " << kTerminal << " black and white" << '\n';
            script_os << "set terminal " << kTerminal << " enhanced monochrome dashed font ',15'" << '\n' << '\n';

            script_os << "replot" << '\n' << '\n';

            script_os << "exit gnuplot" << '\n';

            script_os.close();
        } // End GNUPLOT SCRIPT

        if (0 != std::system(("gnuplot \"" + (current_output_path / path(dataset_name + cfg_.gnuplot_script_extension)).string() + "\" 2> gnuplot_errors.txt").c_str())) {
            ob.Cwarning("Unable to run gnuplot script");
        }
        ob.CloseBox();
    }
}

void ThebeTests::MemoryTest()
{
    // Initialize output message box
    OutputBox ob("Memory Test");

    path current_output_path(cfg_.output_path / path(cfg_.memory_folder));

    String output_file((current_output_path.string() / path(cfg_.memory_file)).string());

    if (!create_directories(current_output_path)) {
        ob.Cwarning("Unable to find/create the output path '" + current_output_path.string() + "', 'memory test' skipped");
        ob.CloseBox();
        return;
    }

    // To write MEMORY results
    ofstream os(output_file);
    if (!os.is_open()) {
        ob.Cwarning("Unable to open '" + output_file + "', 'memory test' skipped");
        ob.CloseBox();
        return;
    }
    os << "#Average number of accesses" << '\n';

    for (unsigned d = 0; d < cfg_.memory_datasets.size(); ++d) { // For every dataset in the average list
        String dataset_name(cfg_.memory_datasets[d]);

        path dataset_path(cfg_.input_path / path(dataset_name)),
            is_path = dataset_path / path(cfg_.input_txt); // files.txt path

        // To save list of filename on which CLLAlgorithms must be tested
        vector<pair<string, bool>> filenames;  // first: filename, second: state of filename (find or not)
        if (!LoadFileList(filenames, is_path)) {
            ob.Cwarning("Unable to open '" + is_path.string() + "', skipped", dataset_name);
            continue;
        }

        // Number of files
        unsigned filenames_size = static_cast<unsigned>(filenames.size());

        unsigned tot_test = 0; // To count the real number of image on which labeling will be applied for every file in list

        // Initialize results container
        // To store average memory accesses (one column for every data_ structure type: col 1 -> BINARY_MAT, col 2 -> LABELED_MAT, col 3 -> EQUIVALENCE_VET, col 0 -> OTHER)
        memory_accesses_[dataset_name] = Mat1d(Size(MD_SIZE, static_cast<unsigned>(cfg_.ccl_mem_algorithms.size())), 0.0);

        // Start output message box
        ob.StartUnitaryBox(dataset_name, filenames_size);

        // For every file in list
        for (unsigned file = 0; file < filenames.size(); ++file) {
            // Display output message box
            ob.UpdateUnitaryBox(file);

            string filename = filenames[file].first;
            path filename_path = dataset_path / path(filename);

            // Read and load image
            if (!GetBinaryImage(filename_path, Thinning::img_)) {
                ob.Cwarning("Unable to open '" + filename + "'");
                continue;
            }

            ++tot_test;

            // For all the Algorithms in the array
            for (unsigned i = 0; i < cfg_.ccl_mem_algorithms.size(); ++i) {
                Thinning *algorithm = ThinningMapSingleton::GetThinning(cfg_.ccl_mem_algorithms[i]);

                // The following data_ structure is used to get the memory access matrices
                vector<uint64_t> accesses; // Rows represents algorithms and columns represent data_ structures

                try {
                    algorithm->PerformThinningMem(accesses);
                }
                catch (const exception& e) {
                    algorithm->FreeThinningData();
                    ob.Cerror("Something wrong with " + cfg_.ccl_mem_algorithms[i] + ": " + e.what()); // You should check your algorithms' implementation before performing THeBE tests  
                }

                // For every data_ structure "returned" by the algorithm
                for (unsigned a = 0; a < accesses.size(); ++a) {
                    memory_accesses_[dataset_name](i, a) += accesses[a];
                }
                algorithm->FreeThinningData();
            }
        }
        ob.StopUnitaryBox();

        // To calculate average memory accesses
        for (int r = 0; r < memory_accesses_[dataset_name].rows; ++r) {
            for (int c = 0; c < memory_accesses_[dataset_name].cols; ++c) {
                memory_accesses_[dataset_name](r, c) /= tot_test;
            }
        }

        os << "#" << dataset_name << '\n';
        os << "Algorithm\tBinary Image\tLabel Image\tEquivalence Vector/s\tOther\tTotal Accesses" << '\n';

        for (unsigned a = 0; a < cfg_.ccl_mem_algorithms.size(); ++a) {
            double total_accesses{ 0.0 };
            os << cfg_.ccl_mem_algorithms[a] << '\t';
            for (int col = 0; col < memory_accesses_[dataset_name].cols; ++col) {
                os << std::fixed << std::setprecision(0) << memory_accesses_[dataset_name](a, col);
                os << '\t';
                total_accesses += memory_accesses_[dataset_name](a, col);
            }

            os << total_accesses;
            os << '\n';
        }

        os << '\n' << '\n';;
    }
    os.close();
}

void ThebeTests::LatexGenerator()
{
    OutputBox ob("Generation of Latex file/s");
    path latex = cfg_.latex_path / path(cfg_.latex_file);
    ofstream os(latex.string());
    if (!os.is_open()) {
        ob.Cwarning("Unable to open/create '" + latex.string() + "', generation skipped");
        return;
    }

    // fixed number of decimal values
    os << fixed;
    os << setprecision(3);

    // Document begin
    os << "%These file is generated by THeBE. Follow our project on GitHub: https://github.com/prittt/THeBE" << '\n' << '\n';
    os << "\\documentclass{article}" << '\n' << '\n';

    os << "\\usepackage{siunitx}" << '\n';
    os << "\\usepackage{graphicx}" << '\n';
    os << "\\usepackage{subcaption}" << '\n';
    os << "\\usepackage[top = 1in, bottom = 1in, left = 1in, right = 1in]{geometry}" << '\n' << '\n';

    os << "\\title{{ \\huge\\bfseries THeBE TESTS}}" << '\n';
    os << "\\date{" + GetDatetime() + "}" << '\n';
    os << "\\author{}" << '\n' << '\n';
    os << "\\begin{document}" << '\n' << '\n';
    os << "\\maketitle" << '\n' << '\n';

    // Section average results table ------------------------------------------------------------------------------------------
    if (cfg_.perform_average) {
        os << "\\section{Average Table Results}" << '\n' << '\n';

        os << "\\begin{table}[tbh]" << '\n' << '\n';
        os << "\t\\centering" << '\n';
        os << "\t\\caption{Average Results in ms (Lower is better)}" << '\n';
        os << "\t\\label{tab:table1}" << '\n';
        os << "\t\\begin{tabular}{|l|";
        for (unsigned i = 0; i < cfg_.ccl_average_algorithms.size(); ++i)
            os << "S[table-format=2.3]|";
        os << "}" << '\n';
        os << "\t\\hline" << '\n';
        os << '\t';
        for (unsigned i = 0; i < cfg_.ccl_average_algorithms.size(); ++i) {
            //RemoveCharacter(datasets_name, '\\');
            //datasets_name.erase(std::remove(datasets_name.begin(), datasets_name.end(), '\\'), datasets_name.end());
            os << " & {" << EscapeUnderscore(cfg_.ccl_average_algorithms[i]) << "}"; //Header
        }
        os << "\\\\" << '\n';
        os << "\t\\hline" << '\n';
        for (unsigned i = 0; i < cfg_.average_datasets.size(); ++i) {
            os << '\t' << cfg_.average_datasets[i];
            for (int j = 0; j < average_results_.cols; ++j) {
                os << " & ";
                if (average_results_(i, j) != numeric_limits<double>::max())
                    os << average_results_(i, j); //Data
            }
            os << "\\\\" << '\n';
        }
        os << "\t\\hline" << '\n';
        os << "\t\\end{tabular}" << '\n' << '\n';
        os << "\\end{table}" << '\n';
    }

    { // CHARTS SECTION ------------------------------------------------------------------------------------------
        SystemInfo s_info(cfg_);
        string info_to_latex = s_info.build() + "_" + s_info.compiler_name() + s_info.compiler_version() + "_" + s_info.os();
        std::replace(info_to_latex.begin(), info_to_latex.end(), ' ', '_');
        info_to_latex = EscapeUnderscore(info_to_latex);

        string chart_size{ "0.45" }, chart_width{ "1" };
        // Get information about date and time
        string datetime = GetDatetime();

        string compiler_name(s_info.compiler_name());
        string compiler_version(s_info.compiler_version());
        //replace the . with _ for compiler strings
        std::replace(compiler_version.begin(), compiler_version.end(), '.', '_');

        // SECTION AVERAGE CHARTS  ---------------------------------------------------------------------------
        if (cfg_.perform_average) {
            os << "\\section{Average Charts}" << '\n' << '\n';
            os << "\\begin{figure}[tbh]" << '\n' << '\n';
            // \newcommand{ \machineName }{x86\_MSVC15.0\_Windows\_10\_64\_bit}
            os << "\t\\newcommand{\\machineName}{";
            os << info_to_latex << "}" << '\n';
            // \newcommand{\compilerName}{MSVC15_0}
            os << "\t\\newcommand{\\compilerName}{" + compiler_name + compiler_version + "}" << '\n';
            os << "\t\\centering" << '\n';

            for (unsigned i = 0; i < cfg_.average_datasets.size(); ++i) {
                os << "\t\\begin{subfigure}[tbh]{" + chart_size + "\\textwidth}" << '\n';
                os << "\t\t\\caption{" << cfg_.average_datasets[i] + "}" << '\n';
                os << "\t\t\\centering" << '\n';
                os << "\t\t\\includegraphics[width=" + chart_width + "\\textwidth]{\\compilerName_" + cfg_.average_datasets[i] + ".pdf}" << '\n';
                os << "\t\\end{subfigure}" << '\n' << '\n';
            }
            os << "\t\\caption{\\machineName \\enspace " + datetime + "}" << '\n' << '\n';
            os << "\\end{figure}" << '\n' << '\n';
        }

        // SECTION AVERAGE WITH STEPS CHARTS  ---------------------------------------------------------------------------
        if (cfg_.perform_average_ws) {
            string average_ws_suffix{ "with_steps_" };

            os << "\\section{Average With Steps Charts}" << '\n' << '\n';
            os << "\\begin{figure}[tbh]" << '\n' << '\n';
            // \newcommand{ \machineName }{x86\_MSVC15.0\_Windows\_10\_64\_bit}
            os << "\t\\newcommand{\\machineName}{";
            os << info_to_latex << "}" << '\n';
            // \newcommand{\compilerName}{MSVC15_0}
            os << "\t\\newcommand{\\compilerName}{" + compiler_name + compiler_version + "}" << '\n';
            os << "\t\\centering" << '\n';
            for (unsigned i = 0; i < cfg_.average_ws_datasets.size(); ++i) {
                os << "\t\\begin{subfigure}[tbh]{" + chart_size + "\\textwidth}" << '\n';
                os << "\t\t\\caption{" << cfg_.average_ws_datasets[i] + "}" << '\n';
                os << "\t\t\\centering" << '\n';
                os << "\t\t\\includegraphics[width=" + chart_width + "\\textwidth]{\\compilerName_" + average_ws_suffix + cfg_.average_ws_datasets[i] + ".pdf}" << '\n';
                os << "\t\\end{subfigure}" << '\n' << '\n';
            }
            os << "\t\\caption{\\machineName \\enspace " + datetime + "}" << '\n' << '\n';
            os << "\\end{figure}" << '\n' << '\n';
        }
    } // END CHARTS SECTION

    // SECTION MEMORY RESULT TABLE ---------------------------------------------------------------------------
    if (cfg_.perform_memory) {
        os << "\\section{Memory Accesses tests}" << '\n' << '\n';
        os << "Analysis of memory accesses required by connected components computation. The numbers are given in millions of accesses." << '\n';

        for (const auto& dataset : memory_accesses_) {
            const auto& dataset_name = dataset.first;
            const auto& accesses = dataset.second;

            os << "\\begin{table}[tbh]" << '\n' << '\n';
            os << "\t\\centering" << '\n';
            os << "\t\\caption{Memory accesses on ``" << dataset_name << "'' dataset}" << '\n';
            os << "\t\\label{tab:table_" << dataset_name << "}" << '\n';
            os << "\t\\begin{tabular}{|l|";
            for (int i = 0; i < accesses.cols + 1; ++i)
                os << "S[table-format=2.3]|";
            os << "}" << '\n';
            os << "\t\\hline" << '\n';
            os << '\t';

            // Header
            os << "{Algorithm} & {Binary Image} & {Label Image} & {Equivalence Vector/s}  & {Other} & {Total Accesses}";
            os << "\\\\" << '\n';
            os << "\t\\hline" << '\n';

            for (unsigned i = 0; i < cfg_.ccl_mem_algorithms.size(); ++i) {
                // For every algorithm escape the underscore
                const String& alg_name = EscapeUnderscore(cfg_.ccl_mem_algorithms[i]);
                //RemoveCharacter(alg_name, '\\');
                os << "\t{" << alg_name << "}";

                double tot = 0;

                for (int s = 0; s < accesses.cols; ++s) {
                    // For every data_ structure

                    os << "\t& " << (accesses(i, s) / 1000000);

                    tot += (accesses(i, s) / 1000000);
                }
                // Total Accesses
                os << "\t& " << tot;

                // EndLine
                os << "\t\\\\" << '\n';
            }

            // EndTable
            os << "\t\\hline" << '\n';
            os << "\t\\end{tabular}" << '\n' << '\n';
            os << "\\end{table}" << '\n';
        }
    }

    os << "\\end{document}";
    os.close();
}