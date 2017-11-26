#pragma once

#include <limits>
#include <memory>

#include "stype.h"
#include "stokenizer.h"

namespace snack {
	enum sttype_spec {
		bool_type = 1024,
		byte_type = 0,
		char_type = 2,
		sword_type = 4,
		short_type = 8,
		int_type = 16,
		word_type = 32,
		lint_type = 64,
		lword_type = 128,
		qword_type = 256,
		qint_type = 512,
		real_type = 2048,
		ext_type = 4096,
		extext_type = 1 << 13,
		pointer_type = 1 << 14,
		ant_type = 1 << 15
	};

	
	typedef void(*node_destroy_func)(void*);

	template <typename T>
	static inline void node_destroy(void* self) {
		((T*)(self))->~T();
		free(self);
	}

	class smem {

		void* data;
		node_destroy_func dfunc;

	public:

		smem();

		template <typename T>
		smem(T* data, node_destroy_func dfunc);

		void destroy();

	};

	struct spos {
		s64 line;
		s64 col;

		s64 pos;
	};


	//template <typename T>
	class stnode {

	protected:

		spos nodepos;
		std::vector<stnode> children;

	public:


		stnode() {};
		stnode(s64 p_line, s64 p_col, s64 p_pos) {
			nodepos.line = p_line;
			nodepos.col = p_col;
			nodepos.pos = p_pos;
		};

		void set_pos(spos pos) {
			nodepos = pos;
		}

		spos get_pos() {
			return nodepos;
		}

		virtual ~stnode() { };
	};

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

	class stconstexp : public stnode {
		sttype_spec type;

		union {
			long long dv;
			long double fv;
			std::string lv;
		};

	public:

		stconstexp() {};
		~stconstexp() {};
	};

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

	struct sttype;

	struct stvar : public stnode {

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
	
	struct sttype : public stnode {

		bool is_built_in;
		bool is_pointer;

		std::string name;

	public:

		sttype(bool is_built_in, bool is_pointer = false);
		bool is_type_built_in();

	};

	struct stbuiltin : public sttype {
		int type;

	public:

		stbuiltin(int type, bool is_pointer);
	};


	class stexp : public stnode {

	protected:

		int type;

	public:

		enum {
			int_constant,
			uint_constant,
			float_constant,
			double_constant,
			ldouble_constant,
			bool_constant,
			short_constant,
			ushort_constant,
			char_constant,
			uchar_constant,
			ll_constant,
			ull_constant,
			var_identifier,
			feild,
			arr_subscript,
			func_call,
			cons_call,
			unary_minus,
			unary_plus,
			bit_not,
			logical_not,
			assign,
			string_constant,
			op,
			sequence,
			prefix_increment,
			prefix_decrement,
			post_increment,
			post_decrement
		};

		stexp(int type);

	};

	class stcall : public stnode {

		std::string callee;
		//std::vector<stexp> arguments;
	};

	struct ststruct : public sttype {

		std::vector<stvar*> fields;

	public:
		ststruct();
	};

	class ststatement;

	class stfunc : public stnode {
		std::vector<ststatement*> statements;
		std::vector<stvar*> parameters;

	public:
		stfunc() {};
	};

}