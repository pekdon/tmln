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

#ifndef _TMLN_SELECTION_HH_
#define _TMLN_SELECTION_HH_

#include "config.h"

#include "tmln_data.hh"

namespace tmln {
	/**
	 * Data selection implementation limiting the events to max
	 * number and a given offset.
	 */
	class NumOffsetSelection : public Data {
	public:
		NumOffsetSelection(const Data &data,
				   unsigned int max_num, size_t offset);
		~NumOffsetSelection();

		virtual const TsSpan& span() const override { return _span; }
		virtual size_t size() const override;
		virtual size_t begin() const override { return _pos_begin; }
		virtual size_t end() const override { return _pos_end; }
		virtual const Event &operator[](size_t idx) const override;
		virtual bool add_event(const Event& event) override
		{
			return false;
		}

		size_t data_size() const { return _data.size(); }
		const TsSpan& data_span() const { return _data.span(); }

		void set_selection(unsigned int max_num, size_t offset);

	private:
		const Data &_data;
		unsigned int _max_num;
		size_t _offset;
		TsSpan _span;

		size_t _pos_begin;
		size_t _pos_end;
	};

	/**
	 * Data selection implementation limiting the events to the
	 * given timespan and a maximum number of events.
	 */
	class NumTimeSelection : public Data {
	public:
		NumTimeSelection(const Data &data,
				 unsigned int max_num, const TsSpan &span);
		~NumTimeSelection();

		virtual const TsSpan& span() const override { return _span; }
		virtual size_t size() const override
		{
			return _pos_end - _pos_begin;
		}
		virtual size_t begin() const override { return _pos_begin; }
		virtual size_t end() const override { return _pos_end; }
		virtual const Event &operator[](size_t idx) const override;
		virtual bool add_event(const Event& event) override
		{
			return false;
		}

		size_t data_size() const;
		void set_selection(unsigned int max_num, const TsSpan &span);

	private:
		const Data &_data;
		unsigned int _max_num;
		TsSpan _span;

		size_t _pos_begin;
		size_t _pos_end;
	};
};


#endif // _TMLN_SELECTION_HH_
