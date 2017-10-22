#include "stexp.h"
#include "sconst.h"

namespace snack {
	int_gen_sconst::int_gen_sconst(long l): stexp(stexp::int_constant), val(l) {
	
		if ((-128 <= l) && (l <= 127)) {
			type = stexp::char_constant;
		}
		else if ((0 <= l) && (l <= 255)) {
			type = stexp::uchar_constant;
		}
		else if ((-32768 <= l) && (l <= 32767)) {
			type = stexp::short_constant;
		}
		else if ((0 <= l) && (l <= 65535)) {
			type = stexp::ushort_constant;
		}
		else if ((-2147483648 <= l) && (l <= 2147483647)) {
			type = stexp::int_constant;
		}
		else if ((0 <= l) && (l <= 4294967295)) {
			type = stexp::uint_constant;
		}
		else if ((-9223372036854775808 <= l) && (l <= 9223372036854775807)) {
			type = stexp::ll_constant;
		}
		else type = stexp::ull_constant;
	}

	long long int_gen_sconst::get_val() const {
		return val;
	}

	float_gen_sconst::float_gen_sconst(long double l) : stexp(stexp::float_constant), val(l) {
		if ((std::numeric_limits<float>::min() <= l) && (l <= std::numeric_limits<float>::max())) {
			type = stexp::float_constant;
		}
		else if ((std::numeric_limits<double>::min() <= l) && (l <= std::numeric_limits<double>::max())) {
			type = stexp::double_constant;
		}
		else if ((std::numeric_limits<long float>::min() <= l) && (l <= std::numeric_limits<long float>::max())) {
			type = stexp::ldouble_constant;
		}
	}

	long double float_gen_sconst::get_val() const {
		return val;
	}

	boolean_gen_sconst::boolean_gen_sconst(bool l) : stexp(stexp::bool_constant), val(l) {};

	bool boolean_gen_sconst::get_val() const {
		return val;
	}


	str_gen_sconst::str_gen_sconst(std::string l) : stexp(stexp::string_constant), val(l) {};

	std::string str_gen_sconst::get_val() const {
		return val;
	}
}