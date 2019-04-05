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