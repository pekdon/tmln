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

#ifndef _TMLN_TIME_HH_
#define _TMLN_TIME_HH_

#include "config.h"

#include <cstdint>
#include <string>

namespace tmln {

	extern const int64_t NSEC_PER_SEC;

	/**
	 * Timestamp type used in Event
	 */
	class Ts {
	public:
		Ts(double sec);
		Ts(int64_t sec, int64_t nsec);
		Ts(const std::string& time_str);
		Ts(const char* time_str);
		~Ts(void);

		int64_t sec() const { return _sec; }
		int64_t nsec() const { return _nsec; }

		double to_sec() const
		{
			return _sec + (_nsec / static_cast<double>(NSEC_PER_SEC));
		}

	private:
		void parse(const char *time_str);

	private:
		int64_t _sec;
		int64_t _nsec;
	};

	bool operator==(const Ts& lhs, const Ts& rhs);
	bool operator<(const Ts& lhs, const Ts& rhs);
	bool operator<=(const Ts& lhs, const Ts& rhs);
	bool operator>(const Ts& lhs, const Ts& rhs);
	bool operator>=(const Ts& lhs, const Ts& rhs);

	Ts operator+(const Ts& lhs, const Ts& rhs);
	Ts operator-(const Ts& lhs, const Ts& rhs);


	/**
	 * Timespan limited by two timestamps.
	 */
	class TsSpan {
	public:
		TsSpan(const Ts& start, const Ts& end);
		~TsSpan();

		double to_sec() const;

		const Ts& start() const { return _start; }
		const Ts& end() const { return _end; }
		bool inside(const Ts& ts) const;

		void set_start(const Ts& start) { _start = start; }
		void set_end(const Ts& end) { _end = end; }

	private:
		Ts _start;
		Ts _end;
	};

	bool operator==(const TsSpan& lhs, const TsSpan& rhs);
}

#endif // _TMLN_TIME_HH_
