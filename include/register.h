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

#ifndef THEBE_REGISTER_H_
#define THEBE_REGISTER_H_

#define REGISTER_THINNING(algorithm)                                           \
class register_##algorithm {                                                   \
  public:                                                                      \
    register_##algorithm() {                                                   \
        ThinningMapSingleton::GetInstance().data_[#algorithm] = new algorithm; \
    }                                                                          \
} reg_##algorithm;

//#define STRINGIFY(x) #x
//#define CONCAT(x,y) STRINGIFY(x ## _ ## y)
//
//#define REGISTER_SOLVER(algorithm, solver)                                                              \
//class register_ ## algorithm ## _ ## solver{                                                            \
//  public:                                                                                               \
//      register_ ## algorithm ## _ ## solver() {                                                         \
//          LabelingMapSingleton::GetInstance().data_[CONCAT(algorithm, solver)] = new algorithm<solver>; \
//  }                                                                                                     \
//}  register_ ## algorithm ## _ ## solver;

#endif // !THEBE_REGISTER_H_