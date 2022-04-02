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
#include <FL/Fl_Group.H>
#include <FL/Fl_Scrollbar.H>

#include "tmln_render.hh"
#include "tmln_scale.hh"
#include "tmln_selection.hh"

namespace tmln {

	class Fl_Timeline_Scrollbar : public Fl_Scrollbar {
	public:
		Fl_Timeline_Scrollbar(uchar type, int x, int y, int w, int h,
				      const char* label = 0);
		virtual ~Fl_Timeline_Scrollbar();

	private:
		 static void value_cb(Fl_Widget* widget, void* data);
	};

	class Fl_Timeline : public Fl_Group
	{
	public:
		Fl_Timeline(int x, int y, int w, int h, const char* label = 0);
		Fl_Timeline(int x, int y, int w, int h, const char* label,
			    Render* render, Scale* scale,
			    NumOffsetSelection* data_sel);
		virtual ~Fl_Timeline();
	
		virtual int handle(int event);
		virtual void resize(int x, int y, int w, int h);
		virtual void draw();
	
		const std::string& info() const { return _info; }
		void set_info(const std::string& info);
		void zoom(double diff);

	private:
		bool has_data() const;
		void update_scrollbar();

		bool handle_button_press(int button, int x, int y);

		void draw_timeline();
		void draw_timeline_time();
		void draw_timeline_info();
		void draw_timeline_events();
		void draw_legend();
	
	private:
		Fl_Timeline_Scrollbar _x_scrollbar;
		Fl_Timeline_Scrollbar _y_scrollbar;

		tmln::Render* _render;
		tmln::Scale* _scale;
		tmln::NumOffsetSelection* _data_sel;

		std::string _info;
		double _zoom;
	};

	int fltk_ui_main(int argc, char* argv[], Render* render, Scale* scale,
			 NumOffsetSelection* data_sel);

}
