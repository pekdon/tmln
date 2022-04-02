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

#include "tmln_render.hh"

tmln::Render::Render(const Data& data, const Scale& scale, Styles& styles)
	: _data(data),
	  _scale(scale),
	  _styles(styles)
{
}

tmln::Render::~Render()
{
}

bool
tmln::Render::find_event(int x, int y,
			 Event const** event_ret,
			 EventStep const** step_ret) const
{
	bool found = false;
	*event_ret = nullptr;
	*step_ret = nullptr;

	int i = y / _scale.event_height();
	if (i > -1 && i < _data.size()) {
		const Event &event = _data[i];
		Event::step_iterator it = event.cbegin();
		for (; it != event.cend(); ++it) {
			int step_x1 = _scale.time_x(it->start());
			if (step_x1 > x) {
				// will never match, all events are after
				// this position
				return found;
			}

			int step_x2 = _scale.time_x(it->end());
			if (x >= step_x1 && x <= step_x2) {
				*event_ret = &event;
				*step_ret = &(*it);
				found = true;
			}
		}
	}

	return found;
}

void
tmln::Render::render(Draw& draw)
{
	Style line_style = _styles.get_style("black");

	render_scale(draw);

	int y = 0;
	for (size_t i = _data.begin(); i < _data.end(); i++) {
		render_event(draw, y, _data[i]);
		draw.line(0, y, _scale.actual_width(), y, line_style);
		y += _scale.event_height();
		draw.line(0, y, _scale.actual_width(), y, line_style);
	}
}

void
tmln::Render::render_scale(Draw& draw)
{
}

void
tmln::Render::render_event(Draw& draw, const int y, const Event& event) const
{
	int x = _scale.time_x(event.start());
	int width = _scale.span_width(event.span());
	int height = _scale.event_height();

	draw.rectangle(x, y, width, height, event.style());
	Event::step_iterator it = event.cbegin();
	for (; it != event.cend(); ++it) {
		render_step(draw, y, *it);
	}
	if (_scale.event_height() >= draw.text_height()) {
		int text_y = y + (_scale.event_height()
				  - draw.text_height()) / 2;
		draw.text(0, text_y, event.label(), event.style());
	}
}

void
tmln::Render::render_step(Draw& draw, const int y, const EventStep& step) const
{
	int x = _scale.time_x(step.start());
	int width = _scale.span_width(step.span());
	int height = _scale.event_height();
	draw.rectangle(x, y, width, height, step.style());
}
