#pragma once

#include "sttype.h"
#include "stvar.h"

namespace snack{
	class ststatement;

	class stfunc: public stnode<stfunc> {
		std::vector<ststatement*> statements;
		std::vector<stvar*> parameters;

	public:
		stfunc(){}; 
	};
}