#pragma once

#include "stype.h"
#include "stexp.h"

#include <string>
#include <memory>

namespace snack {
	class stbin : public stnode {

		std::string op;
		std::unique_ptr<stnode> lhs, rhs;

	public:

		stbin() {};
		/*
		stbin(std::string op, std::unique_ptr<stnode> lhs, std::unique_ptr<stexp> rhs):
			op(op), lhs(std::move(lhs)), rhs(std::move(rhs))
		{

		}*/

	};
}

