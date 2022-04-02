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

#ifndef _TMLN_RENDER_HH_
#define _TMLN_RENDER_HH_

#include "config.h"

#include "tmln_data.hh"
#include "tmln_draw.hh"
#include "tmln_scale.hh"

namespace tmln {
	/**
	 * Render timeline using data from a Data source and drawing
	 * with a Draw instance.
	 */
	class Render {
	public:
		Render(const Data& data, const Scale& scale, Styles& styles);
		~Render();

		void render(Draw& draw);
		bool find_event(int x, int y, Event const** event,
				EventStep const** step) const;

	private:
		void render_scale(Draw& draw);
		void render_event(Draw& draw, const int y,
				  const Event &event) const;
		void render_step(Draw& draw, const int y,
				 const EventStep& step) const;

	private:
		const Data& _data;
		const Scale& _scale;
		Styles _styles;
	};
}

#endif // _TMLN_RENDER_HH_
