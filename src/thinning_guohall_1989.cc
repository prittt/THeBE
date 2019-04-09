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

#include "thinning_guohall_1989.h"

REGISTER_THINNING(GuoHall);
REGISTER_THINNING(GuoHallLUT);

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
