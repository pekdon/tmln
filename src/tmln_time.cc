//
// Copyright (C) 2022 Claes Nästén <pekdon@gmail.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//

#include <iostream>
#include <ctime>
#include <cstring>

#include "tmln_time.hh"

namespace tmln {
	const int64_t NSEC_PER_SEC = 1000000000;
}

// Ts

tmln::Ts::Ts(double sec)
	: _sec(sec),
	  _nsec((sec - _sec) * NSEC_PER_SEC)
{
}

tmln::Ts::Ts(int64_t sec, int64_t nsec)
	: _sec(sec),
	  _nsec(nsec)
{
}

tmln::Ts::Ts(const std::string& time_str)
{
	parse(time_str.c_str());
}

tmln::Ts::Ts(const char* time_str)
{
	parse(time_str);
}

tmln::Ts::~Ts()
{
}

void
tmln::Ts::parse(const char* time_str)
{
	struct tm tm = {0};
	strptime(time_str, "%Y-%m-%dT%H:%M:%S", &tm);
	time_t sec = timegm(&tm);
	_sec = sec;
	const char* dot = strrchr(time_str, '.');
	if (dot == nullptr) {
		_nsec = 0;
	} else {
		std::string dec_str("0.");
		dec_str += (dot + 1);
		_nsec = std::stod(dec_str) * NSEC_PER_SEC;
	}
}

bool
tmln::operator==(const Ts& lhs, const Ts& rhs)
{
	return (lhs.sec() == rhs.sec()) && (lhs.nsec() == rhs.nsec());
}

bool
tmln::operator<(const Ts& lhs, const Ts& rhs)
{
	if (lhs.sec() < rhs.sec()) {
		return true;
	}
	return (lhs.sec() == rhs.sec()) && (lhs.nsec() < rhs.nsec());
}

bool
tmln::operator<=(const Ts& lhs, const Ts& rhs)
{
	return (lhs == rhs) || (lhs < rhs);
}

bool
tmln::operator>(const Ts& lhs, const Ts& rhs)
{
	if (lhs.sec() > rhs.sec()) {
		return true;
	}
	return (lhs.sec() == rhs.sec()) && (lhs.nsec() > rhs.nsec());
}

bool
tmln::operator>=(const Ts& lhs, const Ts& rhs)
{
	return (lhs == rhs) || (lhs > rhs);
}

tmln::Ts
tmln::operator+(const Ts& lhs, const Ts& rhs)
{
	int64_t sec = lhs.sec() + rhs.sec();
	int64_t nsec = lhs.nsec() + rhs.nsec();
	if (nsec > NSEC_PER_SEC) {
		sec += 1;
		nsec -= NSEC_PER_SEC;
	}
	return Ts(sec, nsec);
}

tmln::Ts
tmln::operator-(const Ts& lhs, const Ts& rhs)
{
	int64_t sec = lhs.sec() - rhs.sec();
	int64_t nsec = lhs.nsec() - rhs.nsec();
	if (nsec < 0) {
		sec -= 1;
		nsec += NSEC_PER_SEC;
	}
	return Ts(sec, nsec);
}

// TsSpan

tmln::TsSpan::TsSpan(const Ts& start, const Ts& end)
	: _start(start),
	  _end(end)
{
}

tmln::TsSpan::~TsSpan()
{
}

double
tmln::TsSpan::to_sec() const
{
	Ts diff = _end - _start;
	return diff.to_sec();
}

bool
tmln::TsSpan::inside(const Ts& ts) const
{
	return ts >= _start && ts < _end;
}

bool
tmln::operator==(const tmln::TsSpan& lhs, const tmln::TsSpan& rhs)
{
	return lhs.start() == rhs.start() && lhs.end() == rhs.end();
}
