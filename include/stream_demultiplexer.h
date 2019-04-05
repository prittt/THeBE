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

#ifndef THEBE_STREAM_DEMULTIPLEXER_H_
#define THEBE_STREAM_DEMULTIPLEXER_H_

#include <ostream>
#include <vector>

class StreamDemultiplexer
{
private:
    typedef std::vector<std::ostream*> str_cont;
    str_cont d;
public:

    StreamDemultiplexer() {}

    StreamDemultiplexer(std::ostream& ss) {
        d.push_back(&ss);
    }

    StreamDemultiplexer& put(std::ostream::char_type ch)
    {
        for (str_cont::iterator it = d.begin(); it != d.end(); ++it)
            (*it)->put(ch);
        return *this;
    }

    StreamDemultiplexer& write(const std::ostream::char_type* s, std::streamsize count)
    {
        for (str_cont::iterator it = d.begin(); it != d.end(); ++it)
            (*it)->write(s, count);
        return *this;
    }

    StreamDemultiplexer& flush()
    {
        for (str_cont::iterator it = d.begin(); it != d.end(); ++it)
            (*it)->flush();
        return *this;
    }

    template<typename T>
    StreamDemultiplexer& operator<<(const T& obj)
    {
        for (str_cont::iterator it = d.begin(); it != d.end(); ++it)
            (**it) << obj;
        return *this;
    }

    StreamDemultiplexer& operator<<(std::ios_base& (*func)(std::ios_base&))
    {
        for (str_cont::iterator it = d.begin(); it != d.end(); ++it)
            (**it) << func;
        return *this;
    }

    template<typename CharT, typename Traits>
    StreamDemultiplexer& operator<<(std::basic_ios<CharT, Traits>& (*func)(std::basic_ios<CharT, Traits>&))
    {
        for (str_cont::iterator it = d.begin(); it != d.end(); ++it)
            (**it) << func;
        return *this;
    }

    StreamDemultiplexer& operator<<(std::ostream& (*func)(std::ostream&))
    {
        for (str_cont::iterator it = d.begin(); it != d.end(); ++it)
            (**it) << func;
        return *this;
    }

    void AddStream(std::ostream& ss)
    {
        d.push_back(&ss);
    }
};

#endif // !THEBE_STREAM_DEMULTIPLEXER_H_