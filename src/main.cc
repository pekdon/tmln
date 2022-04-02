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

#include "tmln_draw_cairo.hh"
#include "tmln_load_json.hh"
#include "tmln_render.hh"
#include "tmln_scale.hh"
#include "tmln_selection.hh"

#ifdef HAVE_FLTK

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Output.H>

#include "tmln_fltk.hh"

static void
fltk_cb_quit(Fl_Widget *widget, void *data)
{
	exit(0);
}

static void
fltk_cb_zoom_in(Fl_Widget *widget, void *data)
{
	tmln::Fl_Timeline *timeline = static_cast<tmln::Fl_Timeline*>(data);
	timeline->zoom(-0.1);
}

static void
fltk_cb_zoom_out(Fl_Widget *widget, void *data)
{
	tmln::Fl_Timeline *timeline = static_cast<tmln::Fl_Timeline*>(data);
	timeline->zoom(0.1);
}

static Fl_Output *output;

static void
fltk_cb_info(Fl_Widget *widget, void *data)
{
	tmln::Fl_Timeline *timeline = static_cast<tmln::Fl_Timeline*>(data);
	output->value(timeline->info().c_str());
}

static int
fltk_ui_main(int argc, char *argv[],
	     const tmln::VectorData &data_store,
	     tmln::Styles &styles)
{
	const int width = 1600;
	const int height = 800;

	std::string title("TMLN: ");
	title += data_store.source();
	Fl_Window *window = new Fl_Double_Window(width, height, title.c_str());
	window->resizable(window);

	Fl_Group *btn_group = new Fl_Group(0, 0, width, 20);
	Fl_Button *btn_quit = new Fl_Button(0, 0, 50, 20, "Quit");
	output = new Fl_Output(50, 0, width - 150, 20);
	Fl_Button *btn_zoomo = new Fl_Button(width - 100, 0, 50, 20, "-");
	Fl_Button *btn_zoomi = new Fl_Button(width - 50, 0, 50, 20, "+");
	btn_group->resizable(output);
	btn_group->end();

	tmln::Scale scale(data_store.span(), data_store.size(), 0, 0);
	tmln::NumOffsetSelection data_sel(data_store, scale.num_events(), 0);
	tmln::Render render(data_sel, scale, styles);
	tmln::Fl_Timeline *timeline =
		new tmln::Fl_Timeline(0, 20, width, height - 20, "",
				      &render, &scale, &data_sel);

	window->end();

	btn_quit->callback(fltk_cb_quit, nullptr);
	btn_zoomo->callback(fltk_cb_zoom_out, timeline);
	btn_zoomi->callback(fltk_cb_zoom_in, timeline);
	timeline->callback(fltk_cb_info, timeline);

	window->show(argc, argv);
	return Fl::run();
}

#else // ! HAVE_FLTK

static int
fltk_ui_main(int argc, char *argv[],
	     const tmln::VectorData &data_store,
	     tmln::Styles &styles)
{
	std::cerr << "error: FLTK support not compiled in" << std::endl;
	return 1;
}


#endif // HAVE_FLTK

#ifdef HAVE_CAIRO

#include "tmln_draw_cairo.hh"

static int
cairo_render(int argc, char *argv[],
	     const tmln::VectorData &data_store,
	     tmln::Styles &styles)
{
	const int width = 1600;
	const int height = 800;
	std::string output_path = argc  > 3 ? argv[3] : "render.png";

	tmln::Scale scale(data_store.span(), data_store.size(), 0, 0);
	tmln::NumTimeSelection data_sel(data_store, scale.num_events(),
					scale.span());
	tmln::Render render(data_sel, scale, styles);

	tmln::DrawCairo draw(width, height);
	render.render(draw);
	draw.save_png(output_path);
	

	return 0;
}

# else // ! HAVE_CAIRO


static int
cairo_render(int argc, char *argv[],
	     const tmln::VectorData &data_store,
	     tmln::Styles &styles)
{
	std::cerr << "error: Cairo support not compiled in" << std::endl;
	return 1;
}

#endif // HAVE_CAIRO

static int
usage(const char *name)
{
	std::cout << name << ": [ui|render] data.json (output.png)"
		  << std::endl;
	return 1;
}

int
main(int argc, char *argv[])
{
	if (argc < 3) {
		return usage(argv[0]);
	}

	std::string mode(argv[1]);
	std::string data_path(argv[2]);
	if (mode != "ui" && mode != "render") {
		return usage(argv[0]);
	}

	tmln::VectorData data_store(data_path);
	tmln::Styles styles;
	tmln::LoadJson json(data_store, styles);
	json.load_file(data_path);

	if (mode == "ui") {
		return fltk_ui_main(argc - 3, argv + 3, data_store, styles);
	} else {
		return cairo_render(argc, argv, data_store, styles);
	}
}
