#pragma once

#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <iterator>
#include <set>
#include <map>
#include <stack>

#include "stype.h"

#define PRPRWC 8

namespace snack {

	struct smacro {
		enum kind {
			object,
			function,
			special
		} marco_kind;

		std::string name = "";
		std::vector<token> body;
		std::vector<token> args;

		u32 nargs;

		bool is_varg = false;
		std::function<void(token)> special_marco_handler;
	};

	class tokenizer;

	const std::string preprocess_keywords[PRPRWC] = {
		"def",
		"undef",
		"ndef",
		"not",
		"if",
		"else",
		"elif",
		"prag"
	};

	class spp {
#ifdef _DEBUG
	public:
#endif

		using macro_map = std::map<std::string, smacro>;
		using token_map = std::map<std::string, token>;
		using _bd_sco_mc = std::vector<token>;

		macro_map macros;

		//A pointer to the lexer
		tokenizer* lexer;

		void _glue_push(std::deque<token> _tks, token _tk);
		token _glue_tk(token _tk1, token _tk2);
		std::deque<token> _add_hs(std::deque<token> _tks, std::set<std::string> _hs);
	
		token _stringize(token _tmpl, std::deque<token> _args);
		std::deque<token> _subst(smacro _marc, std::deque<token>_args, std::set<std::string> _hs);

		std::deque<token> _read_one_arg(token tok, bool readall);
		std::deque<token> _read_args(token tok, smacro marc);

		token _expd_nl(token _tk);
		token _expd(token _tk);
		token _expd_al(std::deque<token> _tks, token _tk);

		void _obj_macr_par(token _tk);
		void _func_macr_par(token _tk);

		bool _func_macr_params_par(token _tk, token_map& _params);
		_bd_sco_mc _func_macr_body_par(token_map& _params);

		token _dir_par(token _tk);
		token _define_par(token _tk);
		token _undef_par(token _tk);

		s32 _is_ppkw(token _tk);
		
	};

}