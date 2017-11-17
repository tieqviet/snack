#pragma once

#include <string>
#include <vector>

#include "stype.h"
#include "stexp.h"

namespace snack {

	class stcall : public stnode {

		std::string callee;
		//std::vector<stexp> arguments;

	};
}
