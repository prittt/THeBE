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

#ifndef THEBE_THINNING_ZHANGSUEN_1984_H_
#define THEBE_THINNING_ZHANGSUEN_1984_H_

#include <vector>

#include <opencv2/core.hpp>

#include "thinning_algorithms.h"
#include "thinning_iteration.h"
#include "register.h"

class ZhangSuen : public Thinning {
public:
    inline static bool should_remove_0(uint16_t block);
    inline static bool should_remove_1(uint16_t block);

    THINNING_ITERATION(0);
    THINNING_ITERATION(1);
    PERFORM_THINNING;
};

class ZhangSuenNoPtrs : public ZhangSuen {
public:
    using ZhangSuen::should_remove_0;
    using ZhangSuen::should_remove_1;

    THINNING_ITERATION_OLD(0);
    THINNING_ITERATION_OLD(1);
    PERFORM_THINNING;
};

class ZhangSuenLUT : public Thinning {
public:
    inline static bool should_remove_0(uint16_t block);
    inline static bool should_remove_1(uint16_t block);

    THINNING_ITERATION(0);
    THINNING_ITERATION(1);
    PERFORM_THINNING;
};

class ZhangSuenTree : public Thinning {
public:
    inline static bool thinning_iteration(cv::Mat1b& img, int iter);
    void PerformThinning();
};

class ZhangSuenDrag : public Thinning {
public:
    inline static bool thinning_iteration(cv::Mat1b& img, int iter);
    void PerformThinning();
};

#endif // !THEBE_THINNING_ZHANGSUEN_H_

