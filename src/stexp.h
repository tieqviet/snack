#pragma once

#include <vector>
#include "stype.h"

typedef void(*node_destroy_func)(void*);

namespace snack {

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

	public:
		/*
		void* operator new(size_t size, std::vector<smem> *collector) throw() {
			void* data = std::malloc(size);

			if (data)
				collector->push_back(smem((T*)data, &node_destroy<T>));

			return data;
		}
		*/
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
}