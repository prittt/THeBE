// Copyright (c) 2020, the THeBE contributors, as
// shown by the AUTHORS file. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef THEBE_THINNING_GUOHALL_1989_H_
#define THEBE_THINNING_GUOHALL_1989_H_

#include <vector>

#include <opencv2/core.hpp>

#include "thinning_algorithms.h"
#include "thinning_iteration.h"
#include "register.h"

class GuoHall : public Thinning {
public:
    inline static bool should_remove_0(uint16_t block);
    inline static bool should_remove_1(uint16_t block);

    THINNING_ITERATION(0);
    THINNING_ITERATION(1);
    PERFORM_THINNING;
};

class GuoHallLUT : public Thinning {
public:
    inline static bool should_remove_0(uint16_t block);
    inline static bool should_remove_1(uint16_t block);

    THINNING_ITERATION(0);
    THINNING_ITERATION(1);
    PERFORM_THINNING;
};

class GuoHallTree : public Thinning {
public:
    inline static bool thinning_iteration(cv::Mat1b& img, int iter);
    PERFORM_THINNING_DT
};

class GuoHallSpaghetti : public Thinning {
public:
    inline static bool thinning_iteration(cv::Mat1b& img, int iter);
    PERFORM_THINNING_DT
};

class GuoHallSpaghetti_FREQ_Hamlet : public Thinning {
public:
    inline static bool thinning_iteration(cv::Mat1b& img, int iter);
    PERFORM_THINNING_DT
};

class GuoHallSpaghetti_FREQ_All : public Thinning {
public:
	inline static bool thinning_iteration(cv::Mat1b& img, int iter);
	PERFORM_THINNING_DT
};

class GuoHallSpaghetti_FREQ_AllNoClassical : public Thinning {
public:
	inline static bool thinning_iteration(cv::Mat1b& img, int iter);
	PERFORM_THINNING_DT
};

using GuoHallSpaghettiFreq = GuoHallSpaghetti_FREQ_All;


#endif // !THEBE_THINNING_GUOHALL_1989_H_

