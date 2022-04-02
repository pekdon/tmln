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

#ifndef _TMLN_LOAD_JSON_HH_
#define _TMLN_LOAD_JSON_HH_

#include "config.h"

#include <string>

#include "json11.hpp"

#include "tmln_data.hh"
#include "tmln_style.hh"

namespace tmln {

	/**
	 * Load events from JSON data in the format:
	 *
	 * {"events": [event],
	 *  "styles": [style]}
	 *
	 * event: {"label": string,
	 *         "start": timestamp,
	 *         "end": timestamp,
	 *         "style": style_ref}
	 *
	 * style: {"name": string,
	 *         "bg": color,
	 *         "fg": color}
	 *
	 * color: "#rrggbb" (#00fafa)
	 * timestamp: "YYYY-MM-DDTHH:mm:ss.f" (2022-01-24T15:40:50.332)
	 * style_ref: "style-name" | color
	 *
	 */
	class LoadJson {
	public:
		LoadJson(Data& data, Styles& styles);
		~LoadJson(void);

		bool load_file(const std::string& path);
		bool load(const std::string& in);
		bool load_obj(const json11::Json& obj);

	private:
		void load_event(const json11::Json& obj);
		void load_steps(Event& event,
				const json11::Json::array& steps);
		void load_step(Event& event, const json11::Json& obj);

		void load_style(const json11::Json& obj);

	private:
		Data& _data;
		Styles& _styles;
	};

}

#endif // _TMLN_LOAD_JSON_HH_
