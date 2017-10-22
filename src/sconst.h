#pragma once

#include <limits>

#include "stype.h"
#include "stexp.h"
#include "stokenizer.h"

namespace snack {

	/*
	enum class sconst_kind {
		boolean_const = 1,
		char_const = 2,
		unsigned_char_const = 4,
		short_const = 8,
		unsigned_short_const = 16,
		integer_const = 32,
		unsigned_integer_const = 64,
		long_const = 128,
		unsigned_long_const = 256,
		long_long_const = 512,
		unsigned_long_long_const = 1024,
		string_const = 2048,
		real_const = 4096,
		extended_const = 8192,
		extra_extended_const = 16384
		
	};*/

	class int_gen_sconst: stexp {
		explicit int_gen_sconst(long l);

		long long         get_val() const;

	private:
		long long val;

	};

	class float_gen_sconst : stexp {
		explicit float_gen_sconst(long double l);

		long double         get_val() const;

	private:
		long double val;
	};

	class boolean_gen_sconst : stexp {
		explicit boolean_gen_sconst(bool l);

		bool              get_val() const;

	private:
		bool val;
	};

	class str_gen_sconst : stexp {
		explicit str_gen_sconst(std::string l);

		std::string       get_val() const;

	private:
		std::string val;
	};
}