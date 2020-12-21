// Copyright (c) 2020, the THeBE contributors, as
// shown by the AUTHORS file. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef THEBE_THINNING_ALGORITHMS_H_
#define THEBE_THINNING_ALGORITHMS_H_

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

#include "performance_evaluator.h"

class Thinning {
public:
    static cv::Mat1b img_;
    cv::Mat1b img_out_;
    PerformanceEvaluator perf_;

    Thinning() {}
    virtual ~Thinning() = default;

    virtual void PerformThinning() { throw std::runtime_error("'PerformThinning()' not implemented"); }
    virtual void PerformThinningWithSteps() { throw std::runtime_error("'PerformThinningWithSteps()' not implemented"); }
    virtual void PerformThinningMem(std::vector<uint64_t>& accesses) { throw std::runtime_error("'PerformThinningMem(...)' not implemented"); }

    virtual void FreeThinningData() { img_out_.release(); }
    
};

class ThinningMapSingleton {
public:
    std::map<std::string, Thinning*> data_;

    static ThinningMapSingleton& GetInstance();
    static Thinning* GetThinning(const std::string& s);
    static bool Exists(const std::string& s);
	ThinningMapSingleton(ThinningMapSingleton const&) = delete;
    void operator=(ThinningMapSingleton const&) = delete;

private:
	ThinningMapSingleton() {}
    ~ThinningMapSingleton()
    {
        for (std::map<std::string, Thinning*>::iterator it = data_.begin(); it != data_.end(); ++it)
            delete it->second;
    }
};

enum StepType {
    ALLOC_DEALLOC = 0,
    THINNING = 1,

    ST_SIZE = 2,
};

std::string Step(StepType n_step);

#endif //THEBE_THINNING_ALGORITHMS_H_