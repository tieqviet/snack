#include "spis.h"

namespace snack {
	spis::spis(std::ifstream* p_input,std::string fn): line(1), pos(1), fname(fn) {
		input << p_input->rdbuf();
		newline_counter.insert(1);
	}

	spis::spis(std::stringstream* strstream, std::string fn) : line(1), pos(1), col(1), fname(fn) { 
		input.str(strstream->str());
		newline_counter.insert(1);
	};

	spis::spis(const spis& stream): line(stream.line), pos(stream.pos), col(stream.col), fname(stream.fname) {
		input.str(stream.input.str());
	}

	void spis::set_pos(u32 p_pos) {
		//input.seekg(p_pos);

		u32 i = 1;
		s32 previous = -10;

		bool found = false;

		for (auto linen_pos : newline_counter) {
			if (((p_pos <= linen_pos) && ((s32)p_pos >= previous))) {
				line = i;
				col = linen_pos - p_pos;
				found = true;
				break;
			}

			i++;
			previous = linen_pos;
		}
		
		if (!found)
		{
			std::string pline;

			line = newline_counter.size();
			u32 counter_t = previous;
	
			input.seekg(counter_t);

			while ((p_pos > counter_t) && (!input.eof())) {
				std::getline(input, pline);

				counter_t = u32(input.tellg()) + 1;
				newline_counter.insert(counter_t);

				col = p_pos - counter_t;

				line++;
			}
			
		}
		
		input.clear();
		input.seekg(p_pos -1);
		pos = p_pos;
	}


	std::string spis::data() {
		return input.str();
	}

	s8 spis::next() {
		s8 cf = read_char(input);

		if (input.rdstate() != 0) {
			return -1;
		}

		if (cf == '\n') {
			line++;
			pos++;
			col = 0;

			newline_counter.insert(pos);
		}
		else {
			pos++;
			col++;
		}

		return cf;

	}

	bool spis::append(std::string dat) {
		u64 npos = input.tellp();

		input.clear();

		input.seekp(0, std::ios::end);
		input.write(dat.data(), dat.size());

		input.seekp(npos);

		return true;
	}

	bool spis::write(std::string len, u32 inp_pos) {
		std::string bgn = input.str();
		u64 npos = input.tellp();

		input.seekg(0, std::ios::end);

		try {
			bgn = bgn.substr(inp_pos, bgn.size());
		}
		catch (...) {

		}

		s8 ct;

		input.clear();
		input.seekp(inp_pos-1,std::ios::cur);

		//pos = input.tellg();

		input.write(len.data(), len.size());
		input.write(bgn.data(), bgn.size());

		input.seekp(npos);

		pos = input.tellg(); pos += 1;

		//std::cout << "PARSED MARCO INTO FILE AT POS: " << pos << std::endl;
		return true;
	}

	bool spis::erase(u32 bg, u32 end) {
		std::string ref = input.str();

		u32 pos_af = input.tellg();

		ref.erase(bg-1, end - bg);

		input.str(ref);
		
		if ((bg <= pos_af) && (pos_af <= end)) {
			pos_af = bg;
		}
		else if ((bg <= pos_af) && (end <= pos_af)) {
			pos_af -= (end-bg + 1);
		}

		pos = pos_af+1;

		input.clear();
		input.seekg(pos_af);

		return true;
	}

	std::string spis::readline() {
		std::string tline; std::getline(input, tline);

		line++;
		col = 0;
		pos = u32(input.tellg()) + 1;

		return tline;
	}

	s8 spis::peek() {
		//std::cout << "POS: " << input.tellg() <<"  "<< input.rdstate() << std::endl;
		return input.peek();
	}

	bool spis::eof() {
		return input.eof();
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

	spis::spis() : line(1),col(1),pos(1) { newline_counter.insert(1); };

	void spis::operator = (spis& rhs) {
		input.str(rhs.input.str());

		col = rhs.col;
		line = rhs.line;
		pos = rhs.pos;
		newline_counter = rhs.newline_counter;
	}
}
