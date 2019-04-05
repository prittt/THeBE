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

#ifndef THEBE_THINNING_BOLELLI_H_
#define THEBE_THINNING_BOLELLI_H_

#include <vector>

#include <opencv2/core.hpp>

#include "thinning_algorithms.h"
#include "register.h"

class ZHANGSUEN_DRAG : public Thinning {
public:
	ZHANGSUEN_DRAG() {}

	static bool thinning_iteration_ZS(cv::Mat1b& img, int iter) {
		cv::Mat1b out(img.size(), 0);

		auto w = img.cols;

		bool modified = false;
		for (int r = 1; r < img.rows - 1; r++) {

#define CONDITION_P1 img(r,c)
#define CONDITION_P2 img(r-1,c)
#define CONDITION_P3 img(r-1,c+1)
#define CONDITION_P4 img(r,c+1)
#define CONDITION_P5 img(r+1,c+1)
#define CONDITION_P6 img(r+1,c)
#define CONDITION_P7 img(r+1,c-1)
#define CONDITION_P8 img(r,c-1)
#define CONDITION_P9 img(r-1,c-1)
#define CONDITION_ITER iter

#define ACTION_1 ;                  // keep0
#define ACTION_2 out(r,c) = 1;      // keep1
#define ACTION_3 modified = true;   // change0

			int c = -1;
			goto tree_0;

#include "thinning_bolelli_2019_drag.inc"

		}

		img = out;

#undef CONDITION_P1
#undef CONDITION_P2
#undef CONDITION_P3
#undef CONDITION_P4
#undef CONDITION_P5
#undef CONDITION_P6
#undef CONDITION_P7
#undef CONDITION_P8
#undef CONDITION_P9
#undef CONDITION_ITER

#undef ACTION_1
#undef ACTION_2
#undef ACTION_3

		return modified;
	}

    void PerformThinning()
    {
		img_out_ = img_.clone();
		// The input image should be binary 0 background, 255 foregroung
		//img_out_ /= 255;

		while (true) {
			if (!thinning_iteration_ZS(img_out_, 0))
				break;
			if (!thinning_iteration_ZS(img_out_, 1))
				break;
		}

		img_out_ *= 255;

    }

    void PerformThinningWithSteps()
    {
        //double alloc_timing = Alloc();

        //perf_.start();
        //FirstScan();
        //perf_.stop();
        //perf_.store(Step(StepType::FIRST_SCAN), perf_.last());

        //perf_.start();
        //SecondScan();
        //perf_.stop();
        //perf_.store(Step(StepType::SECOND_SCAN), perf_.last());

        //perf_.start();
        //Dealloc();
        //perf_.stop();
        //perf_.store(Step(StepType::ALLOC_DEALLOC), perf_.last() + alloc_timing);
    }

    void PerformThinningMem(std::vector<uint64_t>& accesses)
    {
       
    }

private:
    
	double Alloc()
	{}

	void Dealloc()
	{}

	void FirstScan()
	{}

	void SecondScan()
	{}
};

#endif // !THEBE_THINNING_BOLELLI_H_

