// Copyright (c) 2020, the THeBE contributors, as
// shown by the AUTHORS file. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

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

class ZhangSuenOpenCV : public Thinning {
public:
    // Applies a thinning iteration to a binary image
    static void thinningIteration(cv::Mat img, int iter) {
        using namespace cv;
        Mat marker = Mat::zeros(img.size(), CV_8UC1);

        memset(marker.ptr<void>(0), 1, img.cols);
        memset(marker.ptr<void>(img.rows - 1), 1, img.cols);
        for (int i = 1; i < img.rows - 1; i++)
        {
            // j = 0
            {
                uchar p2 = img.at<uchar>(i - 1, 0);
                uchar p3 = img.at<uchar>(i - 1, 1);
                uchar p4 = img.at<uchar>(i, 1);
                uchar p5 = img.at<uchar>(i + 1, 1);
                uchar p6 = img.at<uchar>(i + 1, 0);
                uchar p7 = 0;
                uchar p8 = 0;
                uchar p9 = 0;

                int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                    (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                    (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                    (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
                int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
                int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
                int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

                if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
                    marker.at<uchar>(i, 0) = 1;
            }
            for (int j = 1; j < img.cols - 1; j++)
            {
                uchar p2 = img.at<uchar>(i - 1, j);
                uchar p3 = img.at<uchar>(i - 1, j + 1);
                uchar p4 = img.at<uchar>(i, j + 1);
                uchar p5 = img.at<uchar>(i + 1, j + 1);
                uchar p6 = img.at<uchar>(i + 1, j);
                uchar p7 = img.at<uchar>(i + 1, j - 1);
                uchar p8 = img.at<uchar>(i, j - 1);
                uchar p9 = img.at<uchar>(i - 1, j - 1);

                int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                    (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                    (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                    (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
                int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
                int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
                int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

                if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
                    marker.at<uchar>(i, j) = 1;
            }
            // j = img.cols - 1
            {
                uchar p2 = img.at<uchar>(i - 1, img.cols - 1);
                uchar p3 = 0;
                uchar p4 = 0;
                uchar p5 = 0;
                uchar p6 = img.at<uchar>(i + 1, img.cols - 1);
                uchar p7 = img.at<uchar>(i + 1, img.cols - 2);
                uchar p8 = img.at<uchar>(i, img.cols - 2);
                uchar p9 = img.at<uchar>(i - 1, img.cols - 2);

                int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                    (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                    (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                    (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
                int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
                int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
                int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

                if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
                    marker.at<uchar>(i, img.cols - 1) = 1;
            }
        }

        img &= ~marker;
    }

    void PerformThinning() 
    {
        using namespace cv;
        Mat processed = img_.clone();
        // Enforce the range of the input image to be in between 0 - 255
        processed /= 255;

        Mat prev = Mat::zeros(processed.size(), CV_8UC1);
        Mat diff;

        do {
            thinningIteration(processed, 0);
            thinningIteration(processed, 1);
            absdiff(processed, prev, diff);
            processed.copyTo(prev);
        } while (countNonZero(diff) > 0);

        processed *= 255;

        img_out_ = processed;
    }
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
    PERFORM_THINNING_DT
};

class ZhangSuenDrag : public Thinning {
public:
    inline static bool thinning_iteration(cv::Mat1b& img, int iter);
    PERFORM_THINNING_DT
};

#endif // !THEBE_THINNING_ZHANGSUEN_H_

