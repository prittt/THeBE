// Copyright (c) 2020, the THeBE contributors, as
// shown by the AUTHORS file. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef MEMORY_TESTER_H_
#define MEMORY_TESTER_H_

#include "opencv2/core.hpp"

enum memorydatatype {
    // Data structures for "classical" algorithms
    MD_BINARY_MAT = 0,
    MD_LABELED_MAT = 1,
    MD_EQUIVALENCE_VEC = 2,

    // Other data_ structures
    MD_OTHER = 3,

    // Total number of data_ structures in the list
    MD_SIZE = 4,
};

template <typename T>
class MemMat {
public:

    int rows;
    int cols;

	MemMat() {}

    MemMat(cv::Mat_<T> img)
    {
        img_ = img.clone(); // Deep copy
        accesses_ = cv::Mat1i(img.size(), 0);
        rows = img.rows;
        cols = img.cols;
    }

    MemMat(cv::Size size)
    {
        img_ = cv::Mat_<T>(size);
        accesses_ = cv::Mat1i(size, 0);
        rows = size.height;
        cols = size.width;
    }

    MemMat(unsigned rows, unsigned cols) : MemMat(cv::Size(cols, rows)) {}

    MemMat(cv::Size size, const T val)
    {
        img_ = cv::Mat_<T>(size, val);
        accesses_ = cv::Mat1i(size, 1);	// The initialization accesses must be counted
        rows = size.height;
        cols = size.width;
    }

    MemMat(unsigned rows, unsigned cols, const T val) : MemMat(cv::Size(cols, rows), val) {}

    T& operator()(const int r, const int c)
    {
        accesses_.ptr<int>(r)[c]++; // Count access
        return img_.template ptr<T>(r)[c];
    }

    T& operator()(const int x)
    {
        accesses_(x)++; // Count access
        return img_(x);
    }

    cv::Mat_<T> GetImage() const
    {
        return img_.clone();
    }

    cv::Mat1i GetAccessesMat() const
    {
        return accesses_.clone();
    }

    double GetTotalAccesses() const
    {
        return cv::sum(accesses_)[0];
    }

    //~MemMat();  // This is the destructor: declaration

private:
    cv::Mat_<T> img_;
    cv::Mat1i accesses_;
};

template <typename T>
class MemVector {
public:
	MemVector() {}

    MemVector(std::vector<T> vec)
    {
        vec_ = vec;  // Deep copy
        accesses_ = std::vector<int>(vec.size(), 0);
    }

    MemVector(const size_t size)
    {
        vec_ = std::vector<T>(size);
        accesses_ = std::vector<int>(size, 0);
    }

    MemVector(const size_t size, const T val)
    {
        vec_ = std::vector<T>(size, val);
        accesses_ = std::vector<int>(size, 1); // The initilization accesses must be counted
    }

    T& operator[](const int i)
    {
        accesses_[i]++; // Count access
        return vec_[i];
    }

    std::vector<T> GetVector() const
    {
        return vec_;
    }

    std::vector<T> GetAccessesVector() const
    {
        return accesses_;
    }

    double GetTotalAccesses() const
    {
        double tot = 0;
        for (size_t i = 0; i < accesses_.size(); ++i)
        {
            tot += accesses_[i];
        }

        return tot;
    }

    T* GetDataPointer()
    {
        return vec_.data_();
    }

    size_t size()
    {
        return vec_.size();
    }

    void Memiota(size_t begin, size_t end, const T value)
    {
        T _value = value;

        for (size_t i = begin; i < end; ++i)
        {
            vec_[i] = _value++;
            accesses_[i]++;	// increment access
        }
    }

    //~MemVector();  // This is the destructor: declaration

private:
    std::vector<T> vec_;
    std::vector<int> accesses_;
};

#endif // MEMORY_TESTER_H_