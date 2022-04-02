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

#include "tmln_scale.hh"

static const int EVENT_MIN_HEIGHT = 5;
static const int EVENT_HEIGHT = 15;
static const int EVENT_MAX_HEIGHT = 30;

tmln::Scale::Scale(const TsSpan& actual_span, unsigned int actual_num_events,
		   int actual_width, int actual_height)
	: _span(actual_span),
	  _scale(1.0),
	  _start(actual_span.start()),
	  _actual_span(actual_span),
	  _actual_num_events(actual_num_events),
	  _actual_width(0),
	  _actual_height(0)
{
	set_actual_size(actual_width, actual_height);
}

tmln::Scale::~Scale()
{
}

int
tmln::Scale::time_x(const Ts& ts) const
{
	Ts diff = ts - _span.start();
	return _sec_to_pixel * diff.to_sec();
}

unsigned int
tmln::Scale::span_width(const TsSpan& span) const
{
	double sec = span.to_sec();
	return static_cast<unsigned int>(_sec_to_pixel * sec);
}

void
tmln::Scale::set_actual_size(int width, int height)
{
	_actual_width = width;
	_actual_height = height;
	calc_events();
	update_sec_to_pixel_ratio();
}

void
tmln::Scale::set_scale(double scale)
{
	_scale = scale;
	calc_span();
	update_sec_to_pixel_ratio();
	calc_events();
}

void
tmln::Scale::set_start(const Ts& start)
{
	_start = start;
	calc_span();
	calc_events();
}

void
tmln::Scale::calc_span()
{
	Ts end = _actual_span.start()  + Ts(_actual_span.to_sec() * _scale);
	_span = TsSpan(_start, end);
}

void
tmln::Scale::calc_events()
{
	if (_actual_width == 0 || _actual_num_events == 0) {
		_event_height = EVENT_HEIGHT;
		_num_events = 0;
		_sec_to_pixel = 0;
		return;
	}

	_event_height = EVENT_HEIGHT;
	while (_event_height > EVENT_MIN_HEIGHT
	       && _num_events < _actual_num_events) {
		_event_height--;
		_num_events = _actual_height / _event_height;
	}
	if (_scale < 1.0) {
		_event_height = static_cast<double>(_event_height) / _scale;
		if (_event_height > EVENT_MAX_HEIGHT) {
			_event_height = EVENT_MAX_HEIGHT;
		}
	}
}


void
tmln::Scale::set_span(const TsSpan& span)
{
	_span = span;
	update_sec_to_pixel_ratio();
}

void
tmln::Scale::update_sec_to_pixel_ratio()
{
	double sec = _span.to_sec();
	if (sec == 0) {
		_sec_to_pixel = 0.0;
	} else {
		_sec_to_pixel = static_cast<double>(_actual_width) / sec;
	}
}
