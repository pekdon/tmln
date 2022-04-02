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

#include <cstdarg>
#include <iostream>
#include <fstream>
#include <sstream>

#include "json11.hpp"

#include "tmln_load_json.hh"

namespace tmln {
	static const int STRING = (1 << 0);
	static const int OBJ = (1 << 1);
	static const int ARRAY = (1 << 2);
	static const int REQUIRED = (1 << 3);
}

static bool
validate_obj(const json11::Json *obj, ...)
{
	if (obj == nullptr || ! obj->is_object()) {
		return false;
	}

	bool status = true;

	va_list ap;
	va_start(ap, obj);
	const char* field = va_arg(ap, const char*);
	while (status && field != nullptr) {
		void* res = va_arg(ap, void*);
		int flags = va_arg(ap, int);

		const json11::Json& obj_field = (*obj)[field];
		if (obj_field == nullptr || obj_field.is_null()) {
			status = !(flags & tmln::REQUIRED);
		} else if ((flags & tmln::STRING) && obj_field.is_string()) {
			std::string* str_res = static_cast<std::string*>(res);
			*str_res = obj_field.string_value();
		} else if ((flags & tmln::OBJ) && obj_field.is_object()) {
			const json11::Json** json_res =
				static_cast<const json11::Json**>(res);
			*json_res = &obj_field;
		} else if ((flags & tmln::ARRAY) && obj_field.is_array()) {
			const json11::Json::array** json_res =
				static_cast<const json11::Json::array**>(res);
			*json_res = &(obj_field.array_items());
		} else {
			status = false;
		}

		field = va_arg(ap, const char*);
	}
	va_end(ap);

	return status;
};

tmln::LoadJson::LoadJson(Data& data, Styles& styles)
	: _data(data),
	  _styles(styles)
{
}

tmln::LoadJson::~LoadJson(void)
{
}

bool
tmln::LoadJson::load_file(const std::string& path)
{
	std::ifstream ifs(path);
	if (! ifs.is_open()) {
		return false;
	}

	std::string line;
	std::ostringstream buf;
	while (std::getline(ifs, line)) {
		buf << line;
	}

	return load(buf.str());
}

bool
tmln::LoadJson::load(const std::string& in)
{
	std::string err;
	json11::Json json = json11::Json::parse(in, err);
	return load_obj(json);
}

bool
tmln::LoadJson::load_obj(const json11::Json& json)
{
	const json11::Json::array* events, *styles = nullptr;
	if (! validate_obj(&json,
			   "events", &events, ARRAY|REQUIRED,
			   "styles", &styles, ARRAY,
			   nullptr)) {
		return false;
	}

	json11::Json::array::const_iterator it = events->begin();
	for (; it != events->end(); ++it) {
		load_event(*it);
	}

	if (styles) {
		it = styles->begin();
		for (; it != styles->end(); ++it) {
			load_style(*it);
		}
	}

	return true;
}

void
tmln::LoadJson::load_event(const json11::Json& obj)
{
	std::string label, info, start, end, style;
	const json11::Json::array *steps = nullptr;
	if (! validate_obj(&obj,
			   "label", &label, STRING|REQUIRED,
			   "info", &info, STRING,
			   "start", &start, STRING|REQUIRED,
			   "end", &end, STRING|REQUIRED,
			   "steps", &steps, ARRAY,
			   "style", &style, STRING,
			   nullptr)) {
		return;
	}

	const Style& style_ref = style.size() == 0
		? _styles.default_style() : _styles.get_style(style);

	Event event(label, info, start, end, style_ref);
	if (steps != nullptr) {
		load_steps(event, *steps);
	}
	_data.add_event(event);
}

void
tmln::LoadJson::load_steps(Event& event, const json11::Json::array& steps)
{
	json11::Json::array::const_iterator it = steps.begin();
	for (; it != steps.end(); ++it) {
		load_step(event, *it);
	}
}

void
tmln::LoadJson::load_step(Event& event, const json11::Json& obj)
{
	std::string label, info, start, end, style;
	if (! validate_obj(&obj,
			   "label", &label, STRING|REQUIRED,
			   "info", &info, STRING,
			   "start", &start, STRING|REQUIRED,
			   "end", &end, STRING|REQUIRED,
			   "style", &style, STRING,
			   nullptr)) {
		return;
	}

	const Style& style_ref = style.size() == 0
		? _styles.default_style() : _styles.get_style(style);
	event.add_step(label, info, start, end, style_ref);
}

void
tmln::LoadJson::load_style(const json11::Json& obj)
{
	std::string name, fg, bg;
	if (! validate_obj(&obj,
			   "name", &name, STRING|REQUIRED,
			   "fg", &fg, STRING|REQUIRED,
			   "bg", &bg, STRING,
			   nullptr)) {
		return;
	}

	_styles.add_style(Style(name, Color(fg), Color(bg)));
}
