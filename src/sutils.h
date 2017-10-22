#pragma once

/*PRECOMPILED INCLUDING*/
#include <fstream>

/*SOURCE INCLUDING*/
#include "spis.h"

namespace snack {
	s8 read_char(std::stringstream& stream);
	s8 read_char(std::stringstream& stream, u32& pos);

	std::string trim(const std::string& str,
		const std::string& whitespace = " ");

}