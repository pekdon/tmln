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

#include <cstring>

#include "tmln_style.hh"

// Color

tmln::Color::Color(uint8_t _a, uint8_t _r, uint8_t _g, uint8_t _b)
	: a(_a),
	  r(_r),
	  g(_g),
	  b(_b)
{
}

tmln::Color::Color(const Color& color)
	: a(color.a),
	  r(color.r),
	  g(color.g),
	  b(color.b)
{
}

tmln::Color::Color(const std::string& color)
	: a(255),
	  r(0),
	  g(0),
	  b(0)
{
	if (color.size() == 7 && color[0] == '#') {
		int sr, sg, sb;
		if (sscanf(color.c_str(), "#%02x%02x%02x",
			   &sr, &sg, &sb) == 3) {
			r = sr;
			g = sg;
			b = sb;
		}
	} else if (color == "white") {
		r = g = b = 255;
	} else if (color == "grey") {
		r = g = b = 128;
	} else if (color == "red") {
		r = 255;
	} else if (color == "green") {
		g = 255;
	} else if (color == "blue") {
		b = 255;
	}
}

bool
tmln::operator==(const Color& lhs, const Color& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(Color)) == 0;
}

// Style

tmln::Style::Style(const std::string& name,
		   uint8_t a, uint8_t r, uint8_t g, uint8_t b) 
	: _name(name),
	  _fg(a, r, g, b),
	  _bg(255, 0, 0, 0)
{
}

tmln::Style::Style(const std::string& name, const Color& fg, const Color& bg)
	: _name(name),
	  _fg(fg),
	  _bg(bg)
{
}

tmln::Style::Style(const Style& style)
	: _name(style._name),
	  _fg(style._fg),
	  _bg(style._bg)
{
}

tmln::Style::~Style()
{
}

bool
tmln::operator==(const tmln::Style& lhs, const tmln::Style& rhs)
{
	return lhs.name() == rhs.name()
		&& lhs.fg() == rhs.fg()
		&& lhs.bg() == rhs.bg();
}

// Styles

tmln::Styles::Styles()
	: _default_style("default", 255, 0, 0, 0)
{
	Color black(255, 0, 0, 0);
	const char* colors[] =
		{"white", "grey", "black", "red", "green", "blue", nullptr};
	for (const char** color = colors; *color; color++) {
		_styles.emplace(*color, Style(*color, Color(*color), black));
	}
}

tmln::Styles::~Styles()
{
}

bool
tmln::Styles::has_style(const std::string& name)
{
	std::map<std::string, Style>::const_iterator it = _styles.find(name);
	return it != _styles.end();

}

const tmln::Style&
tmln::Styles::get_style(const std::string &name)
{
	std::map<std::string, Style>::const_iterator it = _styles.find(name);
	if (it != _styles.end()) {
		return it->second;
	}

	// missing style, see if the style is an #rrggbb color
	if (name.size() == 7 && name[0] == '#') {
		int r, g, b;
		if (sscanf(name.c_str(), "#%02x%02x%02x", &r, &g, &b) == 3) {
			_styles.emplace(name,
					Style(name, 255, r, g, b));
			it = _styles.find(name);
			return it->second;
		}
	}

	return _default_style;
}

void
tmln::Styles::add_style(const Style& style)
{
	_styles.emplace(style.name(), style);
}
