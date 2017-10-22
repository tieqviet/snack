#include "spis.h"

namespace snack {
	spis::spis(std::ifstream* p_input,std::string fn): line(1), pos(0), fname(fn) {
		input = new std::stringstream(std::ios_base::in | std::ios_base::out);

		input->setf(std::ios_base::in | std::ios_base::out);
		input->operator<<(p_input->rdbuf());
	}

	spis::spis(std::stringstream* strstream, std::string fn) : input(strstream), line(1), pos(0), col(0), fname(fn) {};

	void spis::set_pos(u32 p_pos) {
		input->seekg(p_pos);

		u32 i = 1;

		for (auto linen_pos : newline_counter) {
			if (pos <= linen_pos) {
				line = i;
			}

			i++;
		}

		pos = input->tellg();
	}


	std::string spis::data() {
		return input->str();
	}

	s8 spis::next() {
		s8 cf = read_char(*input);

		if (input->rdstate() != 0) {
			return -1;
		}

		if (cf == '\n') {
			line++;
			pos = input->tellg();
			col = 0;

			newline_counter.insert(pos);
		}
		else {
			pos = input->tellg();
			col++;
		}

		return cf;

	}

	bool spis::write(std::string len, u32 inp_pos) {
		std::string bgn = input->str();

		bgn = bgn.substr(inp_pos, bgn.size());
		s8 ct;

		input->seekp(inp_pos,std::ios::cur);

		//pos = input->tellg();

		input->write(len.data(), len.size());
		input->write(bgn.data(), bgn.size());

		pos = input->tellg();

		std::cout << "PARSED MARCO INTO FILE AT POS: " << pos << std::endl;
		return true;
	}

	bool spis::erase(u32 bg, u32 end) {
		std::string ref = input->str();

		u32 pos_af = input->tellg();

		ref.erase(bg, end - bg + 1);

		input->str(ref);
		
		if ((bg <= pos_af) && (pos_af <= end)) {
			pos_af = bg;
		}
		else if ((bg <= pos_af) && (end <= pos_af)) {
			pos_af -= (end-bg + 1);
		}

		pos = pos_af;

		input->seekg(pos_af);
		input->seekg(pos_af);

		return true;
	}

	std::string spis::readline() {
		std::string tline; std::getline(*input, tline);

		line++;
		col = 0;
		pos = input->tellg();

		return tline;
	}

	s8 spis::peek() {
		std::cout << "POS: " << input->tellg() <<"  "<< input->rdstate() << std::endl;

		return input->peek();
	}

	bool spis::eof() {
		return input->eof();
	}

	u32 spis::get_line_number() {
		return line;
	}

	u32 spis::get_pos_number() {
		return pos;
	}

	u32 spis::get_col_number() {
		return col;
	}

	spis::spis() : line(1) { input = new std::stringstream; };

}
