#pragma once

#include <vector>

#include "stokenizer.h"
#include "sconst.h"
#include "stbin.h"
#include "stcall.h"
#include "stexp.h"
#include "stfunc.h"
#include "stvar.h"


namespace snack {

	
	class sparser {

		/*
		stexp* parse_exp();
		stexp* parse_prim();
		stexp* parse_binop_rhs(int precedence, stexp* lhs);

		
		stexp* parse_unaryop();
		stexp* parse_identifier_exp();
		stexp* parse_paren_exp();

		stexp* parse_real_gen_exp();
		stexp* parse_int_gen_exp();
		stexp* parse_str_gen_exp();

		stexp* parse_statement();

		stexp* parse_block_statement();
		stexp* parse_block_or_statement();
		*/

		bool _is_keyword(token tok);
		bool _is_builtintype(token tok);
		bool _is_op(token tok);
		bool _is_var(token tok);
		bool _is_pun(token tok);
		bool _is_definedtype(token tok, sttype* dtp);
		bool _is_type(token tok);

		std::vector<stfunc*> funcs;
		std::vector<sttype*> defined_types;

		tokenizer lexer;

		sttype* _parse_var_type();
		stvar* _parse_var();
		sttype* _parse_define_statement();

	public:

		explicit sparser(tokenizer& p_lex);
		std::vector<stexp*>& parse();

	};

}