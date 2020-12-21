// Copyright (c) 2020, the THeBE contributors, as
// shown by the AUTHORS file. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "thinning_algorithms.h"

cv::Mat1b Thinning::img_;

ThinningMapSingleton& ThinningMapSingleton::GetInstance()
{
    static ThinningMapSingleton instance;	// Guaranteed to be destroyed.
                                            // Instantiated on first use.
    return instance;
}

Thinning* ThinningMapSingleton::GetThinning(const std::string& s)
{
    return ThinningMapSingleton::GetInstance().data_.at(s);
}

bool ThinningMapSingleton::Exists(const std::string& s)
{
    return ThinningMapSingleton::GetInstance().data_.end() != ThinningMapSingleton::GetInstance().data_.find(s);
}

std::string Step(StepType n_step)
{
    switch (n_step) {
    case ALLOC_DEALLOC:
        return "Alloc Dealloc";
        break;
    case THINNING:
        return "Thinning";
        break;
    case ST_SIZE: // To avoid warning on AppleClang
        break;
    }

    return "";
}