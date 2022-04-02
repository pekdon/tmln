
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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "tmln_load_json.hh"
#include "tmln_time.hh"

// tmln_load_json

class LoadTest {
public:
	LoadTest(const std::string& json)
		: data("memory"),
		  load(data, styles)
	{
		status = load.load(json);
	}

	tmln::VectorData data;
	tmln::Styles styles;
	tmln::LoadJson load;
	bool status;
};

TEST_CASE("test LoadJson load empty object")
{
	LoadTest test("{}");
	CHECK(test.status == false);
	CHECK(test.data.size() == 0);
}

TEST_CASE("test LoadJson load empty events")
{
	LoadTest test("{\"events\": []}");
	CHECK(test.status == true);
	CHECK(test.data.size() == 0);
}

TEST_CASE("test LoadJson load event")
{
	LoadTest test("{\"events\": \
		       [{\"label\": \"label\", \
		         \"info\": \"info\", \
			 \"start\": \"1970-01-01T00:00:01.000\", \
			 \"end\": \"1970-01-01T00:01:02.003\"}]}");
	CHECK(test.status == true);
	CHECK(test.data.size() == 1);
	CHECK(test.data[0] ==
	      tmln::Event("label", "info",
			  tmln::Ts(1, 0), tmln::Ts(62, 3000000),
			  test.styles.default_style()));
}

TEST_CASE("test LoadJson empty string")
{
	LoadTest test("");
	CHECK(test.status == false);
	CHECK(test.data.size() == 0);
}

TEST_CASE("test LoadJson invalid JSON")
{
	LoadTest test("not JSON");
	CHECK(test.status == false);
	CHECK(test.data.size() == 0);
}

TEST_CASE("test LoadJson missing event field")
{
	LoadTest test("{\"events\": [{\"start\": \"\"}]}");
	CHECK(test.status == true);
	CHECK(test.data.size() == 0);
}

TEST_CASE("test LoadJson load style")
{
	LoadTest test("{\"events\": [], \
			\"styles\": \
			[{\"name\": \"example\", \
			  \"fg\": \"#112233\", \
			  \"bg\": \"#0000ff\"}]}");
	CHECK(test.status == true);
	CHECK(test.styles.has_style("example") == true);
	const tmln::Style& style = test.styles.get_style("example");
	CHECK(style.fg() == tmln::Color(255, 17, 34, 51));
	CHECK(style.bg() == tmln::Color(255, 0, 0, 255));
}

TEST_CASE("test LoadJson missing style field")
{
	LoadTest test("{\"events\": [], \
			\"styles\": \
			[{\"name\": \"example\", \
			  \"bg\": \"#0000ff\"}]}");
	CHECK(test.status == true);
	CHECK(test.styles.has_style("example") == false);
}

// tmln_time, Ts

TEST_CASE("test Ts")
{
	CHECK(tmln::Ts(231, 120000000) == tmln::Ts(231.12));
	CHECK(tmln::Ts(0, 0) == tmln::Ts(std::string("1970-01-01T00:00:00")));
	CHECK(tmln::Ts(20, 10000000) == tmln::Ts("1970-01-01T00:00:20.010"));
}

TEST_CASE("test Ts.to_sec")
{
	CHECK(tmln::Ts(0, 0).to_sec() == 0.0);
	CHECK(tmln::Ts(0, 100000000).to_sec() == 0.1);
	CHECK(tmln::Ts(2, 20000000).to_sec() == 2.02);
}

TEST_CASE("test Ts.operator+")
{
	CHECK((tmln::Ts(0, 0) + tmln::Ts(0, 0)).to_sec() == 0.0);
	CHECK((tmln::Ts(1, 0) + tmln::Ts(0, 0)).to_sec() == 1.0);
	CHECK((tmln::Ts(1, 0) + tmln::Ts(0, 100000000)).to_sec() == 1.1);
	CHECK((tmln::Ts(2, 200000000) + tmln::Ts(1, 500000000)).to_sec() == 3.7);
}

TEST_CASE("test Ts.operator-")
{
	CHECK((tmln::Ts(0, 0) - tmln::Ts(0, 0)).to_sec() == 0.0);
	CHECK((tmln::Ts(1, 0) - tmln::Ts(0, 0)).to_sec() == 1.0);
	CHECK((tmln::Ts(1, 0) - tmln::Ts(0, 100000000)).to_sec() == 0.9);
	CHECK((tmln::Ts(2, 200000000) - tmln::Ts(1, 500000000)).to_sec() == 0.7);
}
