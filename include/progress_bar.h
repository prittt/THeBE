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

#ifndef THEBE_PROGRESS_BAR_H_
#define THEBE_PROGRESS_BAR_H_

#include <fstream>
#include <iostream>
#include <iomanip>
#include <ostream>
#include <string>
#include <vector>

#include "file_manager.h"
#include "system_info.h"

#define CONSOLE_WIDTH 80

/* This class is useful to display a progress bar in the output console.
Example of usage:
    progressBar p(number_of_things_to_do);
    p.start();

    //start cycle
        p.display(current_thing_number);
        //do something
    //end cycle

    p.end();
*/
class ProgressBar {
public:

    ProgressBar() {}

    ProgressBar(const unsigned n_things_todo,
        const unsigned gap = 4,
        const unsigned console_width = 70,
        const std::string pre_message = "",
        const std::string post_message = "")
    {
        n_things_todo_ = n_things_todo;

        gap_ = gap;
        console_width_ = console_width;

        pre_message_ = pre_message;
        post_message_ = post_message;

        bar_width_ = console_width_ - static_cast<unsigned>(pre_message_.size()) - static_cast<unsigned>(post_message_.size()) - 5 /* 100%*/ - 2 /*[]*/;
    }

    void Start()
    {
        dmux::cout << pre_message_;
        dmux::cout << "[>";
        for (unsigned i = 0; i < bar_width_ - 1; ++i) {
            dmux::cout << " ";
        }
        dmux::cout << "]   0%";
        dmux::cout << post_message_ << "\r";
        dmux::cout.flush();
        prev_ = 0;
    }

    void Display(const unsigned progress)
    {
        if (progress < n_things_todo_ && prev_ == (gap_ - 1)) {
            std::cout << pre_message_;
            std::cout << "[";
            unsigned pos = bar_width_ * progress / n_things_todo_;
            for (unsigned i = 0; i < bar_width_; ++i) {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << ">";
                else std::cout << " ";
            }
            std::string s = std::to_string(unsigned(progress * 100.0 / n_things_todo_));
            std::cout << "] " << std::string(3 - s.size(), ' ') << s << "%";
            std::cout << post_message_ << "\r";
            std::cout.flush();
            prev_ = 0;
        }
        else {
            prev_++;
        }
    }

    void End()
    {
        dmux::cout << pre_message_;
        dmux::cout << "[";
        for (unsigned i = 0; i < bar_width_; ++i) {
            dmux::cout << "=";
        }
        dmux::cout << "] 100%";
        dmux::cout << post_message_;
        dmux::cout.flush();
        dmux::cout << std::endl;
        prev_ = 0;
    }

    ProgressBar(const unsigned n_things_todo,
        const unsigned n_tests /*min 1 - max 999*/,
        const unsigned gap = 4,
        const unsigned console_width = 70,
        const std::string pre_message = "",
        const std::string post_message = "")
    {
        n_things_todo_ = n_things_todo;

        gap_ = gap;
        console_width_ = console_width;

        pre_message_ = pre_message;
        post_message_ = post_message;

        n_tests_ = n_tests;
        n_tests_length_ = static_cast<unsigned>(std::to_string(n_tests).length());
        bar_width_ = console_width_ - static_cast<unsigned>(pre_message_.size()) - static_cast<unsigned>(post_message_.size()) - 5 /* 100%*/ - 2 /*[]*/ - 5 /*Test */ - (n_tests_length_*2 + 1) /*102/999*/ - 3 /* - */;
    }

    void StartRepeated()
    {
        dmux::cout << pre_message_ << "test " << std::setfill(' ') << std::setw(n_tests_length_) << cur_test_;
        dmux::cout << "/" << n_tests_ << " - ";
        dmux::cout << "[>";
        for (unsigned i = 0; i < bar_width_ - 1; ++i) {
            dmux::cout << " ";
        }
        dmux::cout << "]   0%";
        dmux::cout << post_message_ << "\r";
        dmux::cout.flush();
        prev_ = 0;
    }

    void DisplayRepeated(const unsigned progress)
    {
        if (progress < n_things_todo_ && prev_ == (gap_ - 1)) {
            std::cout << pre_message_ << "test " << std::setfill(' ') << std::setw(n_tests_length_) << cur_test_;
            std::cout << "/" << n_tests_ << " - ";
            std::cout << "[";
            unsigned pos = bar_width_ * progress / n_things_todo_;
            for (unsigned i = 0; i < bar_width_; ++i) {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << ">";
                else std::cout << " ";
            }
            std::string s = std::to_string(unsigned(progress * 100.0 / n_things_todo_));
            std::cout << "] " << std::string(3 - s.size(), ' ') << s << "%";
            std::cout << post_message_ << "\r";
            std::cout.flush();
            prev_ = 0;
        }
        else {
            prev_++;
        }
    }

    bool EndRepeated()
    {
        dmux::cout << pre_message_ << "test " << std::setfill(' ') << std::setw(n_tests_length_) << cur_test_;
        dmux::cout << "/" << n_tests_ << " - ";
        dmux::cout << "[";
        for (unsigned i = 0; i < bar_width_; ++i) {
            dmux::cout << "=";
        }
        dmux::cout << "] 100%";
        dmux::cout << post_message_;
        dmux::cout.flush();
        prev_ = 0;
        if (cur_test_ == n_tests_) {
            dmux::cout << std::endl;
            return true;
        }
        dmux::cout << "\r";
        cur_test_++; 
        return false;
}

private:
    unsigned prev_;
    unsigned n_things_todo_;
    unsigned gap_;
    unsigned bar_width_;
    unsigned console_width_;
    std::string post_message_;
    std::string pre_message_;

    unsigned n_tests_;
    unsigned n_tests_length_;
    unsigned cur_test_ = 1;
};



/* This class is useful to display a title bar in the output console. */
class OutputBox {
public:

    OutputBox(const std::string& title = "", 
        const unsigned bar_width = CONSOLE_WIDTH,
        const unsigned pre_spaces = 2)
    {
        pre_spaces_ = pre_spaces;
        bar_width_ = bar_width > CONSOLE_WIDTH ? CONSOLE_WIDTH : bar_width;
        bar_width_ -= pre_spaces_;

        if (title.size() > bar_width_ - 4) {
            title_ = title.substr(0, bar_width_ - 4 - 3) + "...";
        }
        else {
            title_ = title;
        }
        //transform(title_.begin(), title_.end(), title_.begin(), ::toupper);

        if (title_ != "") {
            dmux::cout << "\n";
            PrintSeparatorLine();
            PrintData(title_);
            PrintSeparatorLine();
        }
    }

    void StartUnitaryBox(const std::string &dataset_name, const unsigned n_things_todo)
    {
        PrintData(dataset_name + ":");
        std::string complete_pre_message = std::string(pre_spaces_, ' ') + "|  ";
        pb = ProgressBar(n_things_todo, 4, bar_width_ + pre_spaces_, complete_pre_message, " |");
        pb.Start();
    }

    void UpdateUnitaryBox(const unsigned progress)
    {
        pb.Display(progress);
    }

    void StopUnitaryBox()
    {
        pb.End();
        PrintSeparatorLine();
    }

    void StartRepeatedBox(const std::string &dataset_name, const unsigned n_things_todo, const unsigned n_test_todo)
    {
        PrintData(dataset_name + ":");
        std::string complete_pre_message = std::string(pre_spaces_, ' ') + "|  ";
        pb = ProgressBar(n_things_todo, n_test_todo, 4, bar_width_ + pre_spaces_, complete_pre_message, " |");
        pb.StartRepeated();
    }

    void UpdateRepeatedBox(const unsigned progress)
    {
        pb.DisplayRepeated(progress);
    }

    void StopRepeatedBox(bool close_box = true)
    {
        if (pb.EndRepeated() && close_box) {
            PrintSeparatorLine();
        }
    }

    void DisplayReport(const std::string &title, const std::vector<std::string> &messagges)
    {
        PrintData(title + ":");
        for (const auto& x : messagges) {
            PrintData(" " + x);
        }
        PrintSeparatorLine();
    }

    void Cerror(const std::string& err, const std::string& title = "")
    {
        std::string complete_err = "";
        if (title != "") {
            PrintData(title + ":");
            complete_err = " ";
        }
        complete_err += " ERROR: [" + err + "]";
        PrintData(complete_err);
        PrintSeparatorLine();
        exit(EXIT_FAILURE);

        /*

        If the title is specified this function will print:

        | title:                                                                     |
        |  ERROR: [err]                                                              |
        +----------------------------------------------------------------------------+

        otherwise:

        |  ERROR: [err]                                                              |
        +----------------------------------------------------------------------------+

        */
    }

    void Cwarning(const std::string& wrn, const std::string& title = "")
    {
        std::string complete_wrn = "";
        if (title != "") {
            PrintData(title + ":");
            complete_wrn = " ";
        }

        complete_wrn += " WARNING: [" + wrn + "]";
        PrintData(complete_wrn);

        if (title != "") {
            PrintSeparatorLine();
        }

        /*

        If the title is specified this function will print:

        | title:                                                                     |
        |  WARNING: [wrn]                                                            |
        +----------------------------------------------------------------------------+

        otherwise:

        |  WARNING: [wrn]                                                            |

        */
    }

    void Cmessage(const std::string& msg)
    {
        std::string complete_msg = " MSG: [" + msg + "]";
        PrintData(complete_msg);

        /*

        This function will print:

        | MSG: [msg]                                                                  |

        */
    }

    void CloseBox() {
        PrintSeparatorLine();
    }

private:
    void PrintSeparatorLine()
    {
        dmux::cout << std::string(pre_spaces_, ' ') << "+" << std::string(bar_width_ - 2, '-') << "+\n";
    }
    void PrintRawData(const std::string &data)
    {
        dmux::cout << std::string(pre_spaces_, ' ') << "| " << data << std::string(bar_width_ - data.size() - 4, ' ') << " |\n";
    }
    void PrintData(const std::string &data)
    {
        unsigned step = bar_width_ - 4;
        std::string tab = "    ";
        for (unsigned i = 0; i < data.length(); i += step) {
            if (i == 0) {
                PrintRawData(data.substr(i, step));
                unsigned tab_size = static_cast<unsigned>(tab.size());
                i += tab_size;
                step -= tab_size;
            }
            else {
                PrintRawData(tab + data.substr(i, step));
            }
        }
    }

    unsigned bar_width_;
    std::string title_;
    unsigned pre_spaces_;
    ProgressBar pb;
};

#endif // !THEBE_PROGRESS_BAR_H_