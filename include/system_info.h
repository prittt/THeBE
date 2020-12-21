// Copyright (c) 2020, the THeBE contributors, as
// shown by the AUTHORS file. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef THEBE_SYSTEM_INFO_H_
#define THEBE_SYSTEM_INFO_H_

#include <iostream>
#include <string>

#include <config_data.h>

#if _WIN32 || _WIN64 || WIN32 || __WIN32__ || __WINDOWS__ || __TOS_WIN__
#ifdef _MSC_VER
#include <intrin.h>
#endif
#ifndef NOMINMAX
#define NOMINMAX // Prevent <Windows.h> header file defines its own macros named max and min
#endif
#include <WINDOWS.h>
#include <lm.h>
#pragma comment(lib, "netapi32.lib")
#define THEBE_WINDOWS
#elif  __gnu_linux__ || __linux__
#define THEBE_LINUX
#include <sys/utsname.h>
#elif  __unix || __unix__
#define THEBE_UNIX
#include <sys/utsname.h>
#elif __APPLE__ || __MACH__ || macintosh || Macintosh || (__APPLE__ && __MACH__)
#include <sys/types.h>
#include <sys/sysctl.h>
#define THEBE_APPLE
#endif

extern struct ConfigData cfg;

/*@brief Retrieve system information

Class that retrieves machine information like the CPU
brand name, the OS used, and the architecture employed.

*/
class SystemInfo {
public:
    SystemInfo(ConfigData& cfg)
    {
        SetBuild();
        SetCpuBrand();
        SetOs(cfg);
        SetOsBit();
        SetCompiler();
    }

    // Return the brand and model of the CPU used
    std::string cpu() { return cpu_; }

    // Return the architecture (x86 or x64) used
    std::string build() { return build_; }

    // Return the Operating System used
    std::string os() { return os_ + " " + os_bit_; }

    // Return the compiler_ used (name and version)
    std::string compiler_name() { return compiler_name_; }
    std::string compiler_version() { return compiler_version_; }

private:
    std::string cpu_;
    std::string build_;
    std::string os_;
    std::string os_bit_;
    std::string compiler_name_;
    std::string compiler_version_;

    void SetCpuBrand();
    void SetBuild();
    void SetOs(ConfigData& cfg);
    void SetOsBit();
    void SetCompiler();
};

#endif // !THEBE_SYSTEM_INFO_H_