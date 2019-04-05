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

#ifndef THEBE_THEBE_TESTS_H_
#define THEBE_THEBE_TESTS_H_

#include <map>
#include <utility>

#include <opencv2/imgproc.hpp>

#include "config_data.h"
#include "file_manager.h"
#include "thinning_algorithms.h"
#include "progress_bar.h"

class ThebeTests {
public:
	ThebeTests(const ConfigData& cfg) : cfg_(cfg) {}

    /*void CheckPerformThinning()
    {
        std::string title = "Checking Correctness of 'PerformThinning()' (8-Connectivity)";
        CheckAlgorithms(title, cfg_.ccl_average_algorithms, &Thinning::PerformThinning);
    }
    void CheckPerformThinningWithSteps()
    {
        std::string title = "Checking Correctness of 'PerformThinningWithSteps()' (8-Connectivity)";
        CheckAlgorithms(title, cfg_.ccl_average_ws_algorithms, &Thinning::PerformThinningWithSteps);
    }
    void CheckPerformThinningMem()
    {
        std::string title = "Checking Correctness of 'PerformThinningMem()' (8-Connectivity)";
        std::vector<uint64_t> unused;
        CheckAlgorithms(title, cfg_.ccl_mem_algorithms, &Thinning::PerformThinningMem, unused);
    }*/

    void AverageTest();
    void AverageTestWithSteps();
    void DensityTest();
    void MemoryTest();
    void LatexGenerator();
    void GranularityTest();

private:
    ConfigData cfg_;
    cv::Mat1d average_results_;
    cv::Mat1d density_results_;
    std::map<cv::String, cv::Mat> granularity_results_;
    std::map<cv::String, cv::Mat1d> average_ws_results_; // String for dataset_name, Mat1d for steps results
    std::map<cv::String, cv::Mat1d> memory_accesses_; // String for dataset_name, Mat1d for memory accesses

    bool LoadFileList(std::vector<std::pair<std::string, bool>>& filenames, const filesystem::path& files_path);
    bool CheckFileList(const filesystem::path& base_path, std::vector<std::pair<std::string, bool>>& filenames);
    bool SaveBroadOutputResults(std::map<cv::String, cv::Mat1d>& results, const std::string& o_filename, const cv::Mat1i& labels, const std::vector<std::pair<std::string, bool>>& filenames, const std::vector<cv::String>& ccl_algorithms);
    bool SaveBroadOutputResults(const cv::Mat1d& results, const std::string& o_filename, const cv::Mat1i& labels, const std::vector<std::pair<std::string, bool>>& filenames, const std::vector<cv::String>& ccl_algorithms);
    void SaveAverageWithStepsResults(const std::string& os_name, const cv::String& dataset_name, bool rounded);

    //template <typename FnP, typename... Args>
    //void CheckAlgorithms(const std::string& title, const std::vector<cv::String>& ccl_algorithms, const FnP func, Args&&... args)
    //{
    //    OutputBox ob(title);

    //    std::vector<bool> stats(ccl_algorithms.size(), true);  // True if the i-th algorithm is correct, false otherwise
    //    std::vector<std::string> first_fail(ccl_algorithms.size());  // Name of the file on which algorithm fails the first time
    //    bool stop = false; // True if all the algorithms are not correct

    //    for (unsigned i = 0; i < cfg_.check_datasets.size(); ++i) { // For every dataset in the check_datasets list
    //        cv::String dataset_name(cfg_.check_datasets[i]);
    //        path dataset_path(cfg_.input_path / path(dataset_name));
    //        path is_path = dataset_path / path(cfg_.input_txt); // files.txt path

    //        // Load list of images on which ccl_algorithms must be tested
    //        std::vector<std::pair<std::string, bool>> filenames; // first: filename, second: state of filename (find or not)
    //        if (!LoadFileList(filenames, is_path)) {
    //            ob.Cwarning("Unable to open '" + is_path.string() + "'", dataset_name);
    //            continue;
    //        }

    //        // Number of files
    //        unsigned filenames_size = static_cast<unsigned>(filenames.size());
    //        ob.StartUnitaryBox(dataset_name, filenames_size);

    //        for (unsigned file = 0; file < filenames_size && !stop; ++file) { // For each file in list
    //            ob.UpdateUnitaryBox(file);

    //            std::string filename = filenames[file].first;
    //            path filename_path = dataset_path / path(filename);

    //            // Load image
    //            if (!GetBinaryImage(filename_path, Thinning::img_)) {
    //                ob.Cmessage("Unable to open '" + filename + "'");
    //                continue;
    //            }

    //            unsigned n_labels_correct, n_labels_to_control;

    //            // SAUF with Union-Find is the reference: labels are already "normalized"
    //            auto& sauf = ThinningMapSingleton::GetInstance().data_.at("SAUF_UF");
    //            sauf->PerformThinning();
    //            n_labels_correct = sauf->n_labels_;
    //            cv::Mat1i labeled_img_correct = sauf->img_labels_.clone();
    //            sauf->FreeThinningData();

    //            //cv::Mat1i labeled_img_correct;
    //            //n_labels_correct = cv::connectedComponents(Thinning::img_, labeled_img_correct, 8, 4, cv::CCL_WU);

    //            unsigned j = 0;
    //            for (const auto& algo_name : ccl_algorithms) {
    //                Thinning *algorithm = ThinningMapSingleton::GetThinning(algo_name);

    //                // Perform labeling on current algorithm if it has no previously failed
    //                if (stats[j]) {
    //                    cv::Mat1i& labeled_img_to_control = algorithm->img_labels_;

    //                    (algorithm->*func)(std::forward<Args>(args)...);
    //                    n_labels_to_control = algorithm->n_labels_;

    //                    NormalizeLabels(labeled_img_to_control);
    //                    const auto diff = CompareMat(labeled_img_correct, labeled_img_to_control);

    //                    algorithm->FreeThinningData(); // Free algorithm's data

    //                    if (n_labels_correct != n_labels_to_control || !diff) {
    //                        stats[j] = false;
    //                        first_fail[j] = (path(dataset_name) / path(filename)).string();

    //                        // Stop check test if all the algorithms fail
    //                        if (adjacent_find(stats.begin(), stats.end(), std::not_equal_to<int>()) == stats.end()) {
    //                            stop = true;
    //                            break;
    //                        }
    //                    }
    //                }
    //                ++j;
    //            } // For all the Algorithms in the array
    //        }// END WHILE (LIST OF IMAGES)
    //        ob.StopUnitaryBox();
    //    }// END FOR (LIST OF DATASETS)

    //     // To display report of correctness test
    //    std::vector<std::string> messages(ccl_algorithms.size());
    //    unsigned longest_name = static_cast<unsigned>(max_element(ccl_algorithms.begin(), ccl_algorithms.end(), CompareLengthCvString)->length());

    //    unsigned j = 0;
    //    for (const auto& algo_name : ccl_algorithms) {
    //        messages[j] = "'" + algo_name + "'" + std::string(longest_name - algo_name.size(), '-');
    //        if (stats[j]) {
    //            messages[j] += "-> correct!";
    //        }
    //        else {
    //            messages[j] += "-> NOT correct, it first fails on '" + first_fail[j] + "'";
    //        }
    //        ++j;
    //    }
    //    ob.DisplayReport("Report", messages);
    //}
};

#endif // !THEBE_THEBE_TESTS_H_
