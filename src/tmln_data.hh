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

#ifndef _TMLN_DATA_HH_
#define _TMLN_DATA_HH_

#include "config.h"

#include <string>
#include <vector>

#include "tmln_time.hh"
#include "tmln_style.hh"

namespace tmln {

	/**
	 * Step of Event.
	 */
	class EventStep {
	public:
		EventStep(const std::string& label, const std::string& info,
			  const Ts& start, const Ts& end,
			  const Style& style);
		EventStep(const EventStep& step);
		~EventStep();

		const std::string& label() const { return _label; }
		const std::string& info() const { return _info; }
		const TsSpan& span() const { return _span; }
		const Ts& start() const { return _span.start(); }
		const Ts& end() const { return _span.end(); }
		const Style& style() const { return _style; }

	private:
		std::string _label;
		std::string _info;
		TsSpan _span;
		const Style& _style;
	};

	bool operator==(const EventStep& lhs, const EventStep& rhs);

	/**
	 * Single event in the timeline consisting of multiple steps.
	 */
	class Event {
	public:
		typedef std::vector<EventStep> step_vector;
		typedef std::vector<EventStep>::const_iterator step_iterator;

		Event(const std::string& label, const std::string& info,
		      const Ts& start, const Ts& end, const Style& style);
		Event(const Event& event);
		~Event();

		const std::string& label() const { return _label; }
		const std::string& info() const { return _info; }
		const TsSpan& span() const { return _span; }
		const Ts& start() const { return _span.start(); }
		const Ts& end() const { return _span.end(); }
		const Style& style() const { return _style; }

		const step_vector& steps() const { return _steps; }
		step_iterator cbegin() const { return _steps.cbegin(); }
		step_iterator cend() const { return _steps.cend(); }

		void add_step(const std::string& label,
			      const std::string& info,
			      const Ts& start, const Ts& end,
			      const Style &style);

	private:
		std::string _label;
		std::string _info;
		TsSpan _span;
		const Style& _style;

		step_vector _steps;
	};

	/**
	 * Interface for accessing timeline data.
	 */
	class Data {
	public:
		Data(const std::string& source);
		virtual ~Data();

		virtual const std::string& source() const { return _source; }
		virtual const TsSpan& span() const = 0;
		virtual size_t size() const = 0;
		virtual size_t begin() const = 0;
		virtual size_t end() const = 0;
		virtual const Event& operator[](size_t idx) const = 0;
		virtual bool add_event(const Event& event) = 0;

	private:
		std::string _source;
	};

	bool operator==(const Event& lhs, const Event& rhs);

	/**
	 * std::vector backed Data implementation
	 */
	class VectorData : public Data
	{
	public:
		typedef std::vector<Event> event_vector;

		VectorData(const std::string& source);
		virtual ~VectorData();

		virtual const TsSpan& span() const override { return _span; }
		virtual size_t size() const override { return _data.size(); };
		virtual size_t begin() const override { return 0; };
		virtual size_t end() const override { return _data.size(); }
		virtual const Event& operator[](size_t idx) const override;
		virtual bool add_event(const Event& event) override;

	private:
		TsSpan _span;
		event_vector _data;
	};
};

#endif // _TMLN_DATA_HH_
