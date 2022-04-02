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

#ifndef _TMLN_SCALE_HH_
#define _TMLN_SCALE_HH_

#include "config.h"

#include "tmln_time.hh"

namespace tmln {
	/**
	 * Size conversion utility for timeline.
	 */
	class Scale {
	public:
		Scale(const TsSpan& actual_span,
		      unsigned int actual_num_events,
		      int actual_width, int actual_height);
		~Scale();

		const TsSpan& span() const { return _span; }
		unsigned int num_events() const { return _num_events; }

		unsigned int event_height() const { return _event_height; }
		int time_x(const Ts& ts) const;
		unsigned int span_width(const TsSpan& span) const;

		int actual_width() const { return _actual_width; }
		int actual_height() const { return _actual_height; }

		void set_actual_size(int width, int height);
		void set_scale(double scale);
		void set_start(const Ts& start);

	private:
		void calc_span();
		void calc_events();

		void set_span(const TsSpan& span);
		void update_sec_to_pixel_ratio();

	private:
		TsSpan _span;
		unsigned int _num_events;
		unsigned int _event_height;
		double _sec_to_pixel;

		double _scale;
		Ts _start;

		TsSpan _actual_span;
		unsigned int _actual_num_events;
		int _actual_width;
		int _actual_height;
	};
};

#endif // _TMLN_SCALE_HH_
