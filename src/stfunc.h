#pragma once

#include "sttype.h"
#include "stvar.h"

namespace snack{
	class ststatement;

	class stfunc: public stnode {
		std::vector<ststatement*> statements;
		std::vector<stvar*> parameters;

	public:
		stfunc(){}; 
	};
}