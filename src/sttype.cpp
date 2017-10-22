#include "sttype.h"

namespace snack {

	sttype::sttype(bool is_built_in,bool p_is_pointer): is_built_in(is_built_in), is_pointer(p_is_pointer) {}

	bool sttype::is_type_built_in() {
		return is_built_in;
	}

	stbuiltin::stbuiltin(int type, bool is_pointer): sttype(true, is_pointer), type(type) {

	}

}