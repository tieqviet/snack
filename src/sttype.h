#pragma once

#include "stype.h"
#include "stexp.h"

namespace snack {

	enum sttype_spec {
		bool_type = 1024,
		byte_type = 0,
		char_type = 2,
		sword_type = 4,
		short_type = 8,
		int_type = 16,
		word_type = 32,
		lint_type = 64,
		lword_type = 128,
		qword_type = 256,
		qint_type = 512,
		real_type = 2048,
		ext_type = 4096,
		extext_type = 1 << 13,
		pointer_type = 1 << 14,
		ant_type = 1 << 15
	};

	struct sttype : public stnode<sttype> {

		bool is_built_in;
		bool is_pointer;

		std::string name;

	public:

		sttype(bool is_built_in, bool is_pointer = false);
		bool is_type_built_in();

	};

	struct stbuiltin : public sttype {
		int type;

	public:

		stbuiltin(int type,bool is_pointer);
	};


}