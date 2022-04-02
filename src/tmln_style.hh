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

#ifndef _TMLN_STYLE_HH_
#define _TMLN_STYLE_HH_

#include "config.h"

#include <cstdint>
#include <map>
#include <string>

namespace tmln {

	/**
	 * Single ARGB color definition.
	 */
	struct Color {
		Color(uint8_t _a, uint8_t _r, uint8_t _g, uint8_t _b);
		Color(const Color& color);
		Color(const std::string& color);

		void get(uint8_t& a, uint8_t& r, uint8_t& g, uint8_t& b) const
		{
			a = this->a;
			r = this->r;
			g = this->g;
			b = this->b;
		}

		uint8_t a;
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	bool operator==(const Color& lhs, const Color& rhs);

	/**
	 * Styling of timeline events and steps.
	 */
	class Style {
	public:
		Style(const std::string &name,
		      uint8_t a, uint8_t r, uint8_t g, uint8_t b);
		Style(const std::string& name,
		      const Color& fg, const Color& bg);
		Style(const Style& style);
		~Style();

		const std::string& name() const { return _name; }
		const Color& fg() const { return _fg; }
		const Color& bg() const { return _bg; }

		void get_fg(uint8_t& a,
			    uint8_t& r, uint8_t& g, uint8_t& b) const
		{
			_fg.get(a, r, g, b);
		}
		void get_bg(uint8_t& a,
			    uint8_t& r, uint8_t& g, uint8_t& b) const
		{
			_bg.get(a, r, g, b);
		}

	private:
		std::string _name;
		Color _fg;
		Color _bg;
	};

	bool operator==(const Style& lhs, const Style& rhs);

	/**
	 * Default timline styles.
	 */
	class Styles {
	public:
		Styles();
		~Styles();

		const Style& default_style() const { return _default_style; }
		bool has_style(const std::string& name);
		const Style& get_style(const std::string& name);
		void add_style(const Style& style);

	private:
		Style _default_style;
		std::map<std::string, Style> _styles;
	};
};

#endif // _TMLN_STYLE_HH_
