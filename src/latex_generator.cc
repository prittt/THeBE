// Copyright (c) 2020, the THeBE contributors, as
// shown by the AUTHORS file. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "latex_generator.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "utilities.h"
#include "file_manager.h"
#include "system_info.h"

using namespace cv;
using namespace std;
using namespace filesystem;