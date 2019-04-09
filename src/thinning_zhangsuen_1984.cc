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

#include "thinning_zhangsuen_1984.h"

REGISTER_THINNING(ZhangSuen);
REGISTER_THINNING(ZhangSuenNoPtrs);
REGISTER_THINNING(ZhangSuenLUT);
REGISTER_THINNING(ZhangSuenTree);
REGISTER_THINNING(ZhangSuenDrag);

#define BLOCK_TO_P						\
    const uchar p2 = (block >> 1) & 1;  \
    const uchar p3 = (block >> 2) & 1;  \
    const uchar p4 = (block >> 5) & 1;  \
    const uchar p5 = (block >> 8) & 1;  \
    const uchar p6 = (block >> 7) & 1;  \
    const uchar p7 = (block >> 6) & 1;  \
    const uchar p8 = (block >> 3) & 1;  \
    const uchar p9 = (block >> 0) & 1;  \

inline bool ZhangSuen::should_remove_0(uint16_t block) 
{
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
inline bool ZhangSuen::should_remove_1(uint16_t block) 
{
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

inline bool ZhangSuenLUT::should_remove_0(uint16_t block)
{
    static bool ZhangSuen_LUT0[512] = {
    0,0,0,1,0,0,1,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,1,1,0,1,0,1,0,0,0,1,
    0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,1,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,0,1,1,0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,1,0,0,0,0,
    1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
    };
    return ZhangSuen_LUT0[block];
}
inline bool ZhangSuenLUT::should_remove_1(uint16_t block)
{
    static bool ZhangSuen_LUT1[512] = {
    0,0,0,1,0,0,1,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,1,1,0,1,0,1,0,0,0,1,
    0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,
    1,0,0,0,1,0,1,0,1,1,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,1,0,0,1,0,0,0,
    };
    return ZhangSuen_LUT1[block];
}

inline bool ZhangSuenTree::thinning_iteration(cv::Mat1b& img, int iter)
{
    cv::Mat1b out(img.size(), 0);

    auto w = img.cols;
    auto h = img.rows;

    bool modified = false;
    for (int r = 1; r < img.rows - 1; r++) {
        const unsigned char* const img_row = img.ptr<unsigned char>(r);
        const unsigned char* const img_row_prev = (unsigned char *)(((char *)img_row) - img.step.p[0]);
        const unsigned char* const img_row_foll = (unsigned char *)(((char *)img_row) + img.step.p[0]);
        unsigned char* const out_row = out.ptr<unsigned char>(r);

#define CONDITION_P1 img_row[c]
#define CONDITION_P2 img_row_prev[c]
#define CONDITION_P3 c+1<w && img_row_prev[c+1]
#define CONDITION_P4 c+1<w && img_row[c+1] 
#define CONDITION_P5 c+1<w && img_row_foll[c+1]
#define CONDITION_P6 img_row_foll[c]
#define CONDITION_P7 c-1>=0 && img_row_foll[c-1]
#define CONDITION_P8 c-1>=0 && img_row[c-1]
#define CONDITION_P9 c-1>=0 && img_row_prev[c-1]
#define CONDITION_ITER iter

#define ACTION_1 ;                  // keep0
#define ACTION_2 out_row[c] = 1;    // keep1
#define ACTION_3 modified = true;   // change0

        for (int c = 0; c < img.cols; ++c) {

#include "thinning_zhangsuen_1984_tree.inc"

        }
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
void ZhangSuenTree::PerformThinning()
{
    // The input image should be binary (0 background, 255 foreground)
    img_out_ = img_.clone() / 255;

    while (true) {
        if (!thinning_iteration(img_out_, 0))
            break;
        if (!thinning_iteration(img_out_, 1))
            break;
    }

    img_out_ *= 255;
}

inline bool ZhangSuenDrag::thinning_iteration(cv::Mat1b& img, int iter)
{
    cv::Mat1b out(img.size(), 0);

    auto w = img.cols;

    bool modified = false;
    for (int r = 1; r < img.rows - 1; r++) {
        const unsigned char* const img_row = img.ptr<unsigned char>(r);
        const unsigned char* const img_row_prev = (unsigned char *)(((char *)img_row) - img.step.p[0]);
        const unsigned char* const img_row_foll = (unsigned char *)(((char *)img_row) + img.step.p[0]);
        unsigned char* const out_row = out.ptr<unsigned char>(r);

#define CONDITION_P1 img_row[c]
#define CONDITION_P2 img_row_prev[c]
#define CONDITION_P3 img_row_prev[c+1]
#define CONDITION_P4 img_row[c+1]
#define CONDITION_P5 img_row_foll[c+1]
#define CONDITION_P6 img_row_foll[c]
#define CONDITION_P7 img_row_foll[c-1]
#define CONDITION_P8 img_row[c-1]
#define CONDITION_P9 img_row_prev[c-1]
#define CONDITION_ITER iter

#define ACTION_1 ;                  // keep0
#define ACTION_2 out_row[c] = 1;    // keep1
#define ACTION_3 modified = true;   // change0

        int c = -1;
        goto tree_0;

#include "thinning_zhangsuen_1984_drag.inc"

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
void ZhangSuenDrag::PerformThinning()
{
    // The input image should be binary (0 background, 255 foreground)
    img_out_ = img_.clone() / 255;

    while (true) {
        if (!thinning_iteration(img_out_, 0))
            break;
        if (!thinning_iteration(img_out_, 1))
            break;
    }

    img_out_ *= 255;
}
