#pragma once

#include <string>

#include "stype.h"
#include "sttype.h"
#include "stexp.h"

namespace snack {

	class stconstexp;

	enum {
		const_qual,
		in_qual,
		out_qual,
		inout_qual,
	};

	enum {
		violate_mem_qual = 2,
		read_only_mem_qual = 4,
		write_only_mem_qual = 8
	};

	struct stvar: public stnode<stvar> {

		std::string name;
		int		    type;

		sttype*     base_type;

		bool is_array;
		bool is_pointer;

		stconstexp* intial_const_val;
		std::vector<stconstexp*> array_sizes;

		enum {
			parameter,
			func,
			global,
			field
		};

		stvar() {};
		stvar(const std::string& name, int type) : name(name), type(type) {};
		stvar(int type) : type(type) {};

		void set_name(std::string& p_name) {
			name = p_name;
		}

		std::string get_name() {
			return name;
		}

		sttype* get_base_type() {
			return base_type;
		}

		bool is_var_array() {
			return is_array;
		}

		bool is_var_pointer() {
			return is_pointer;
		}

	};

}


