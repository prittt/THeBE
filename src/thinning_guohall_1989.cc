// Copyright (c) 2020, the THeBE contributors, as
// shown by the AUTHORS file. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "thinning_guohall_1989.h"

REGISTER_THINNING(GuoHall);
REGISTER_THINNING(GuoHallLUT);
REGISTER_THINNING(GuoHallTree);
REGISTER_THINNING(GuoHallSpaghetti);
REGISTER_THINNING(GuoHallSpaghetti_FREQ_Hamlet);
REGISTER_THINNING(GuoHallSpaghetti_FREQ_All);
REGISTER_THINNING(GuoHallSpaghetti_FREQ_AllNoClassical);
REGISTER_THINNING(GuoHallSpaghettiFreq);


#define BLOCK_TO_P						\
    const uchar p2 = (block >> 1) & 1;  \
    const uchar p3 = (block >> 2) & 1;  \
    const uchar p4 = (block >> 5) & 1;  \
    const uchar p5 = (block >> 8) & 1;  \
    const uchar p6 = (block >> 7) & 1;  \
    const uchar p7 = (block >> 6) & 1;  \
    const uchar p8 = (block >> 3) & 1;  \
    const uchar p9 = (block >> 0) & 1;  \

inline bool GuoHall::should_remove_0(uint16_t block) 
{
    BLOCK_TO_P

    int C = ((!p2) && (p3 || p4)) + ((!p4) && (p5 || p6)) +
        ((!p6) && (p7 || p8)) + ((!p8) && (p9 || p2));
    int N1 = (p9 || p2) + (p3 || p4) + (p5 || p6) + (p7 || p8);
    int N2 = (p2 || p3) + (p4 || p5) + (p6 || p7) + (p8 || p9);
    int N = N1 < N2 ? N1 : N2;
    int m = (p6 || p7 || (!p9)) && p8;

    return (C == 1) && ((N >= 2) && (N <= 3)) && (m == 0);
}
inline bool GuoHall::should_remove_1(uint16_t block)
{
    BLOCK_TO_P

    int C = ((!p2) && (p3 || p4)) + ((!p4) && (p5 || p6)) +
        ((!p6) && (p7 || p8)) + ((!p8) && (p9 || p2));
    int N1 = (p9 || p2) + (p3 || p4) + (p5 || p6) + (p7 || p8);
    int N2 = (p2 || p3) + (p4 || p5) + (p6 || p7) + (p8 || p9);
    int N = N1 < N2 ? N1 : N2;
    int m = (p2 || p3 || (!p5)) && p4;

    return (C == 1) && ((N >= 2) && (N <= 3)) && (m == 0);
}

inline bool GuoHallLUT::should_remove_0(uint16_t block)
{
    static bool GuoHall_LUT0[512] = {
    0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,
    0,0,1,1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,1,1,0,0,0,1,0,0,0,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,1,1,1,0,1,1,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,1,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,1,1,1,0,1,1,0,0,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,0,1,0,0,0,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,1,1,1,0,1,1,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,1,0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,
    };
    return GuoHall_LUT0[block];
}
inline bool GuoHallLUT::should_remove_1(uint16_t block)
{
    static bool GuoHall_LUT1[512] = {
    0,0,0,0,0,0,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,
    1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,
    1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,
    };
    return GuoHall_LUT1[block];
}

inline bool GuoHallTree::thinning_iteration(cv::Mat1b& img, int iter)
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

#include "thinning_guohall_1989_tree.inc"

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

inline bool GuoHallSpaghetti::thinning_iteration(cv::Mat1b& img, int iter)
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
        goto cl_tree_0;

#include "GH_Spaghetti_center_line_forest_code.inc.h"

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

inline bool GuoHallSpaghetti_FREQ_Hamlet::thinning_iteration(cv::Mat1b& img, int iter)
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
        goto cl_tree_0;

#include "GH_Spaghetti_FREQ_hamlet_center_line_forest_code.inc.h"

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

inline bool GuoHallSpaghetti_FREQ_All::thinning_iteration(cv::Mat1b& img, int iter)
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
		goto cl_tree_0;
		
#include "GH_Spaghetti_FREQ_fingerprints-hamlet-3dpes-xdocs-tobacco800-mirflickr-medical-classical_center_line_forest_code.inc.h"

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

inline bool GuoHallSpaghetti_FREQ_AllNoClassical::thinning_iteration(cv::Mat1b& img, int iter)
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
		goto cl_tree_0;			

#include "GH_Spaghetti_FREQ_fingerprints-hamlet-3dpes-xdocs-tobacco800-mirflickr-medical_center_line_forest_code.inc.h"

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
