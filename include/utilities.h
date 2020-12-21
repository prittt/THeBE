// Copyright (c) 2020, the THeBE contributors, as
// shown by the AUTHORS file. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef THEBE_UTILITIES_H_
#define THEBE_UTILITIES_H_

#include <string>
#include <opencv2/core.hpp>

#include "file_manager.h"
#include "stream_demultiplexer.h"

extern const std::string kTerminal;
extern const std::string kTerminalExtension;

extern struct ConfigData cfg;

namespace dmux {
    extern class StreamDemultiplexer cout;
}

// This function is useful to delete eventual carriage return from a string
// and is especially designed for windows file newline format
//void DeleteCarriageReturn(std::string& s);
void RemoveCharacter(std::string& s, const char c);

// This function take a char as input and return the corresponding int value (not ASCII one)
unsigned ctoi(const char& c);

// This function help us to manage '\' escape character
//void EraseDoubleEscape(std::string& str);

/*@brief Get information about date and time

@param[in] bool if true substitute both ' ' and ':' chars with '_' and '.'

@return string value with datetime stringyfied
*/
std::string GetDatetime();
std::string GetDatetimeWithoutSpecialChars();

// Get binary image given a image's filename;
bool GetBinaryImage(const std::string& filename, cv::Mat1b& binary_mat);
bool GetBinaryImage(const filesystem::path& p, cv::Mat1b& binary_mat);

// Compare two int matrices element by element
bool CompareMat(const cv::Mat1i& mat_a, const cv::Mat1i& mat_b);

/*@brief Read bool from YAML configuration file

@param[in] node_list FileNode that contain bool data_
@return bool value of field in node_list
*/
//bool ReadBool(const cv::FileNode& node_list);

// Hide blinking cursor from console
void HideConsoleCursor();

int RedirectCvError(int status, const char* func_name, const char* err_msg, const char* file_name, int line, void*);

/*
@brief Return the string title to insert in gnuplot charts
@return string which represents the title
*/
std::string GetGnuplotTitle(ConfigData& cfg);

std::string EscapeUnderscore(const std::string& s);
std::string DoubleEscapeUnderscore(const std::string& s);

#endif // !THEBE_UTILITIES_H_