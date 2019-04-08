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

#ifndef THEBE_THINNING_ZHANGSUEN_H_
#define THEBE_THINNING_ZHANGSUEN_H_

#include <vector>

#include <opencv2/core.hpp>

#include "thinning_algorithms.h"
#include "thinning_iteration.h"
#include "register.h"

class ZS : public Thinning {
public:
	ZS() {}

#define BLOCK_TO_P						\
    const uchar p2 = (block >> 1) & 1;  \
    const uchar p3 = (block >> 2) & 1;  \
    const uchar p4 = (block >> 5) & 1;  \
    const uchar p5 = (block >> 8) & 1;  \
    const uchar p6 = (block >> 7) & 1;  \
    const uchar p7 = (block >> 6) & 1;  \
    const uchar p8 = (block >> 3) & 1;  \
    const uchar p9 = (block >> 0) & 1;  \

	inline static bool should_remove_0(uint16_t block) {
		BLOCK_TO_P

		int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
				(p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
				(p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
				(p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
		int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
		int m1 = p2 * p4 * p6;
		int m2 = p4 * p6 * p8;

		return A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0;
	}
	inline static bool should_remove_1(uint16_t block) {
		BLOCK_TO_P

		int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
				(p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
				(p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
				(p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
		int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
		int m1 = p2 * p4 * p8;
		int m2 = p2 * p6 * p8;

		return A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0;
	}

	THINNING_ITERATION_OLD(0)
	THINNING_ITERATION_OLD(1)

    void PerformThinning()
    {
		cv::Mat1b img_input = img_.clone();
		img_out_ = cv::Mat1b(img_.size());
		// The input image should be binary 0 background, 255 foregroung
		//img_out_ /= 255;

		while (true) {
			if (!thinning_iteration_0(img_input, img_out_))
				break;
			if (!thinning_iteration_1(img_out_, img_input))
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

#endif // !THEBE_THINNING_ZHANGSUEN_H_

