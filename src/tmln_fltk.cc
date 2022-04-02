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

#include <FL/Fl.H>
#include <FL/fl_draw.H>

#include <iostream>

#include "tmln_draw.hh"
#include "tmln_fltk.hh"
#include "tmln_render.hh"
#include "tmln_scale.hh"
#include "tmln_selection.hh"

namespace tmln {
	class DrawFltk : public Draw {
	public:
		DrawFltk(int x_off, int y_off);
		virtual ~DrawFltk();

		virtual int text_height() const { return 10; }

		virtual void line(int x1, int y1, int x2, int y2,
				  const Style& style);
		virtual void rectangle(int x, int y, int width, int height,
				       const Style &style);
		virtual void text(int x, int y, const std::string &str,
				  const Style &style);
	
	private:
		int rx(int x) const { return x + _x_off; }
		int ry(int y) const { return y + _y_off; }

		void set_style(const Style &style);

	private:
		int _x_off;
		int _y_off;
	};
	
	DrawFltk::DrawFltk(int x_off, int y_off)
		: _x_off(x_off),
		  _y_off(y_off)
	{
	}
	
	DrawFltk::~DrawFltk()
	{
	}
	
	void
	DrawFltk::line(int x1, int y1, int x2, int y2,
			       const Style &style)
	{
		set_style(style);
		fl_line(rx(x1), ry(y1), rx(x2), ry(y2));
	}
	
	void
	DrawFltk::rectangle(int x, int y, int width, int height,
				    const Style &style)
	{
		set_style(style);
		fl_rectf(rx(x), ry(y), width, height);
	}
	
	void
	DrawFltk::text(int x, int y, const std::string &str,
			       const Style &style)
	{
		fl_color(FL_BLACK);
		fl_draw(str.c_str(), rx(x), ry(y + 10));
	}

	void
	DrawFltk::set_style(const Style &style)
	{
		uint8_t a, r, g, b;
		style.get_fg(a, r, g, b);
		fl_color(r, g, b);
	}
}

tmln::Fl_Timeline_Scrollbar::Fl_Timeline_Scrollbar(uchar init_type,
						   int x, int y, int w, int h,
						   const char *label)
	: Fl_Scrollbar(x, y, w, h, label)
{
	type(init_type);
	callback(value_cb, static_cast<void*>(this));
}

tmln::Fl_Timeline_Scrollbar::~Fl_Timeline_Scrollbar()
{
}

void
tmln::Fl_Timeline_Scrollbar::value_cb(Fl_Widget *widget, void *data)
{
	Fl_Timeline_Scrollbar *scrollbar =
		static_cast<Fl_Timeline_Scrollbar*>(data);
	scrollbar->parent()->redraw();
}

tmln::Fl_Timeline::Fl_Timeline(int x, int y, int w, int h, const char *label)
	: Fl_Group(x, y, w, h, label),
	  _x_scrollbar(FL_HORIZONTAL, x, y + h - 20, w - 20, 20),
	  _y_scrollbar(FL_VERTICAL, x + w - 20, y, 20, h - 20),
	  _render(nullptr),
	  _scale(nullptr),
	  _data_sel(nullptr),
	  _zoom(1.0)
{
	end();
}

tmln::Fl_Timeline::Fl_Timeline(int x, int y, int w, int h, const char *label,
			       tmln::Render *render, tmln::Scale *scale,
			       tmln::NumOffsetSelection *data_sel)
	: Fl_Group(x, y, w, h, label),
	  _x_scrollbar(FL_HORIZONTAL, x, y + h - 20, w - 20, 20),
	  _y_scrollbar(FL_VERTICAL, x + w - 20, y, 20, h - 20),
	  _render(render),
	  _scale(scale),
	  _data_sel(data_sel),
	  _zoom(1.0)
{
	if (has_data()) {
		_scale->set_actual_size(w - 20, h - 20);
		update_scrollbar();
	}
	end();
}

tmln::Fl_Timeline::~Fl_Timeline()
{
}

int
tmln::Fl_Timeline::handle(int event)
{
	if (event == FL_PUSH
	    && handle_button_press(Fl::event_button(),
				   Fl::event_x(), Fl::event_y())) {
		return 1;
	}
	return Fl_Group::handle(event);
}

bool
tmln::Fl_Timeline::handle_button_press(int button, int x, int y)
{
	if (! has_data() || x >= (w() - 20)) {
		return false;
	}

	if (button == 1) {
		// select event/stage
		const Event *event;
		const EventStep *step;
		if (_render->find_event(x, y - 20, &event, &step)) {
			std::string info =
				event->label() + " " + step->label() + " "
				+ std::to_string(step->span().to_sec()) + "s";
			set_info(info);
			return true;
		}
		return false;
	} else if (button == 3) {
		// clear text
		set_info("");
	}

	return false;
}

void
tmln::Fl_Timeline::resize(int x, int y, int w, int h)
{
	Fl_Group::resize(x, y, w, h);
	_x_scrollbar.resize(x, y + h - 20, w - 20, 20);
	_y_scrollbar.resize(x + w - 20, y, 20, h - 20);
	_scale->set_actual_size(w - 20, h - 20);
	update_scrollbar();
}

void
tmln::Fl_Timeline::draw()
{
	fl_color(FL_WHITE);
	fl_rectf(x(), y(), w(), h());

	if (has_data()) {
		draw_timeline();
	}

	Fl_Widget *const*a = array();
	for (int i = children(); i --; a++) {
		draw_child(**a);
	}

	// clear out area between the end of the scrollbars, timeline will
	// overflow there
	fl_color(FL_GRAY);
	fl_rectf(x() + w() - 20, y() + h() - 20, 20, 20);
}

bool
tmln::Fl_Timeline::has_data() const
{
	if (_render == nullptr || _scale == nullptr) {
		return false;
	}
	return _data_sel != nullptr && _data_sel->data_size() > 0;
}

void
tmln::Fl_Timeline::update_scrollbar()
{
	int x_num = static_cast<int>(_scale->span().to_sec());
	int x_tot = static_cast<int>(_data_sel->data_span().to_sec());
	_x_scrollbar.value(0, x_num, 0, x_tot);
	_y_scrollbar.value(0, _scale->num_events(), 0, _data_sel->data_size());
	
}

void
tmln::Fl_Timeline::draw_timeline()
{
	Ts offset = Ts(_x_scrollbar.value());
	_scale->set_start(_data_sel->data_span().start() + offset);
	size_t trans_offset = _y_scrollbar.value();
	_data_sel->set_selection(_scale->num_events(), trans_offset);
	tmln::DrawFltk draw(x(), y());
	_render->render(draw);
}

void
tmln::Fl_Timeline::draw_timeline_time()
{
}

void
tmln::Fl_Timeline::draw_timeline_info()
{
}

void
tmln::Fl_Timeline::draw_timeline_events()
{
}

void
tmln::Fl_Timeline::set_info(const std::string& info)
{
	if (info == _info) {
		return;
	}
	_info = info;
	do_callback();
}

void
tmln::Fl_Timeline::zoom(double diff)
{
	_zoom += diff;
	if (_zoom > 1) {
		_zoom = 1;
	} else if (_zoom < 0.1) {
		_zoom = 0.1;
	}

	if (has_data()) {
		_scale->set_scale(_zoom);
		update_scrollbar();
	}

	redraw();
}

