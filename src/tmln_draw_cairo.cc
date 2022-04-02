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

#include "tmln_draw_cairo.hh"

tmln::DrawCairo::DrawCairo(int width, int height)
	: _width(width),
	  _height(height)
{
	_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
					      _width, _height);
	_cr = cairo_create(_surface);
	cairo_scale(_cr, _width, _height);
}

tmln::DrawCairo::~DrawCairo()
{
	cairo_destroy(_cr);
	cairo_surface_destroy(_surface);
}

void
tmln::DrawCairo::line(int x1, int y1, int x2, int y2, const Style& style)
{
	set_style(style);
	cairo_move_to(_cr, rel_x(x1), rel_y(y1));
	cairo_line_to(_cr, rel_x(x2), rel_y(y2));
	cairo_stroke(_cr);
}

void
tmln::DrawCairo::rectangle(int x, int y, int width, int height,
			   const Style& style)
{
	set_style(style);
	cairo_rectangle(_cr, rel_x(x), rel_y(y), rel_x(width), rel_y(height));
	cairo_fill(_cr);
}

void
tmln::DrawCairo::text(int x, int y, const std::string& str, const Style& style)
{
	set_style(style);
	cairo_move_to(_cr, rel_x(x), rel_y(y));
	cairo_show_text(_cr, str.c_str());
	cairo_stroke(_cr);
}

bool
tmln::DrawCairo::save_png(const std::string& path)
{
	cairo_status_t status =
		cairo_surface_write_to_png(_surface, path.c_str());
	return status == CAIRO_STATUS_SUCCESS;
}

void
tmln::DrawCairo::set_style(const Style& style)
{
	uint8_t a, r, g, b;
	style.get_fg(a, r, g, b);
	cairo_set_source_rgb(_cr, r, g, b);
	cairo_set_line_width(_cr, 0.1);
}

double
tmln::DrawCairo::rel_x(int x) const
{
	return static_cast<double>(x) / _width;
}

double
tmln::DrawCairo::rel_y(int y) const
{
	return static_cast<double>(y) / _height;
}
