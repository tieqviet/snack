#pragma once

/*PRECOMPILED INCLUDING*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>

/*SOURCE INCLUDING*/
#include "stype.h"
#include "sutils.h"

namespace snack {

	/* INPUT STREAM */
	class spis {

		std::stringstream* input;

		std::string fname;

		std::multiset<u32> newline_counter;

		u32 pos;
		u32 line;
		u32 col;

	public:

		bool write(std::string len, u32 pos);
		bool erase(u32 bg, u32 end);

		std::string name() {
			return fname;
		}

		spis(std::ifstream* input, std::string fn = std::string());
		spis(std::stringstream* strstream, std::string fn = std::string());
		spis();

		s8 next();
		s8 peek();

		std::string data();
		std::string readline();

		u32 get_line_number();
		u32 get_pos_number();
		u32 get_col_number();

		bool eof();

		void set_pos(u32 pos);

	};

}