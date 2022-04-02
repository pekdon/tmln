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

#include "tmln_data.hh"

// EventStep

tmln::EventStep::EventStep(const std::string& label, const std::string& info,
			   const Ts& start, const Ts& end, const Style& style)
	: _label(label),
	  _span(start, end),
	  _style(style)
{
}

tmln::EventStep::EventStep(const EventStep& event)
	: _label(event._label),
	  _info(event._info),
	  _span(event._span),
	  _style(event._style)
{
}

tmln::EventStep::~EventStep()
{
}

bool
tmln::operator==(const EventStep& lhs, const EventStep& rhs)
{
	return lhs.label() == rhs.label()
		&& lhs.info() == rhs.info()
		&& lhs.span() == rhs.span()
		&& lhs.style() == rhs.style();
}

// Event

tmln::Event::Event(const std::string& label, const std::string& info,
		   const Ts& start, const Ts& end, const Style& style)
	: _label(label),
	  _info(info),
	  _span(start, end),
	  _style(style)
{
}

tmln::Event::Event(const Event &event)
	: _label(event._label),
	  _info(event._info),
	  _span(event._span),
	  _style(event._style),
	  _steps(event._steps)
{
}

tmln::Event::~Event()
{
}

void
tmln::Event::add_step(const std::string& label, const std::string& info,
		      const Ts& start, const Ts& end, const Style& style)
{
	_steps.emplace_back(EventStep(label, info, start, end, style));
}

bool
tmln::operator==(const Event& lhs, const Event& rhs)
{
	return lhs.label() == rhs.label()
		&& lhs.info() == rhs.info()
		&& lhs.span() == rhs.span()
		&& lhs.style() == rhs.style()
		&& lhs.steps() == rhs.steps();
}

// Data

tmln::Data::Data(const std::string& source)
	: _source(source)
{
}

tmln::Data::~Data()
{
}

// VectorData

tmln::VectorData::VectorData(const std::string& source)
	: Data(source),
	  _span(Ts(0, 0), Ts(0, 0))
{
}

tmln::VectorData::~VectorData()
{
}

const tmln::Event&
tmln::VectorData::operator[](size_t idx) const
{
	return _data[idx];
}

bool
tmln::VectorData::add_event(const Event &event)
{
	_data.push_back(event);
	if (_data.size() == 1) {
		_span = event.span();
	} else {
		_span.set_end(event.end());
	}
	return true;
}
