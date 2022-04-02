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

#include "tmln_selection.hh"

// NumOffssetSelection

tmln::NumOffsetSelection::NumOffsetSelection(const tmln::Data &data,
					     unsigned int max_num,
					     size_t offset)
	: Data(data.source()),
	  _data(data),
	  _max_num(max_num),
	  _offset(offset),
	  _span(Ts(0, 0), Ts(0, 0))
{
	set_selection(max_num, offset);
}

tmln::NumOffsetSelection::~NumOffsetSelection()
{
}

size_t
tmln::NumOffsetSelection::size() const
{
	return _pos_end - _pos_begin;
}

const tmln::Event&
tmln::NumOffsetSelection::operator[](size_t idx) const
{
	return _data[idx];
}

/**
 * Update selection based on current max number and span.
 */
void
tmln::NumOffsetSelection::set_selection(unsigned int max_num, size_t offset)
{
	if (_max_num == max_num && _offset == offset) {
		return;
	}

	_max_num = max_num;
	_offset = offset;
	_pos_begin = offset;
	if (_offset > _data.size()) {
		_pos_begin = _data.size();
	}
	_pos_end = _offset + max_num;
	if (_pos_end > _data.size()) {
		_pos_end = _data.size();
	}

	if (_pos_begin == _pos_end) {
		_span = TsSpan(Ts(0, 0), Ts(0, 0));
	} else {
		_span = TsSpan(_data[_pos_begin].start(),
			       _data[_pos_end - 1].end());
	}
}

// NumTimeSelection

tmln::NumTimeSelection::NumTimeSelection(const tmln::Data &data,
					 unsigned int max_num,
					 const tmln::TsSpan &span)
	: Data(data.source()),
	  _data(data),
	  _max_num(max_num),
	  _span(span)
{
	set_selection(max_num, span);
}

tmln::NumTimeSelection::~NumTimeSelection()
{
}

const tmln::Event&
tmln::NumTimeSelection::operator[](size_t idx) const
{
	return _data[idx];
}

size_t
tmln::NumTimeSelection::data_size() const
{
	return _data.size();
}

/**
 * Update selection based on current max number and span. 
 */
void
tmln::NumTimeSelection::set_selection(unsigned int max_num, const TsSpan &span)
{
	_max_num = max_num;
	_span = span;
	_pos_begin = _data.end();
	_pos_end = _data.end();

	unsigned int found = 0;
	for (size_t i = _data.begin(); i < _data.end(); i++) {
		const Event &event = _data[i];

		if (event.start() > _span.end()) {
			// past the end of the span, no need to scan any
			// further as nothing will match
			break;
		}

		if (_span.inside(event.start())) {
			if (_pos_begin == _data.end()) {
				_pos_begin = i;
			}
			_pos_end = i + 1;
			if (++found == _max_num) {
				break;
			}
		}
	}
}
