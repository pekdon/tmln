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

#ifndef _TMLN_DRAW_HH_
#define _TMLN_DRAW_HH_

#include "config.h"

#include <string>

#include "tmln_style.hh"

namespace tmln {
	/**
	 * Draw primitives interface
	 */
	class Draw {
	public:
		Draw() { }
		virtual ~Draw() { }

		virtual int text_height() const = 0;

		virtual void line(int x1, int y1, int x2, int y2,
				  const Style& style) = 0;
		virtual void rectangle(int x, int y, int width, int height,
				       const Style& style) = 0;
		virtual void text(int x, int y, const std::string& str,
				  const Style& style) = 0;
	};
}

#endif // _TMLN_DRAW_HH_
