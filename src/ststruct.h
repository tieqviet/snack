#pragma once

#include "sttype.h"
#include "stvar.h"

namespace snack {

	struct ststruct: public sttype {

		std::vector<stvar*> fields;
		
	public:
		ststruct();
	};

}