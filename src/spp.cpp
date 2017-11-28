#include "spp.h"
#include "stokenizer.h"

namespace snack {
	std::deque<token> spp::_add_hs(std::deque<token> _tks, 
		std::set<std::string> _hs) {
		for (auto& _k : _tks) {
			std::set<std::string> ths = _k.hidesets;
			ths.insert(_hs.begin(), _hs.end());
			_k.hidesets = ths;
		}

		return _tks;
	}

	token spp::_glue_tk(token _tk1, token _tk2) {
		std::string _tkc = _tk1.value + _tk2.value;
		std::stringstream _strst(_tkc);
		snack::spis _st(&_strst);
		tokenizer _tktkc(_st);

		return _tktkc.read_next();
	}

	void spp::_glue_push(std::deque<token> _tks, token _tk) {
		token last = *_tks.end();

		_tks.pop_back();
		_tks.push_back(_glue_tk(last, _tk));
	}

	token spp::_stringize(token _tmpl, std::deque<token> _args) {
		_tmpl.value = "";

		for (auto& _arg : _args) {
			_tmpl.value += _arg.value + " ";
		}

		return _tmpl;
	}

	std::deque<token> spp::_subst(smacro _marc, std::deque<token>_args, std::set<std::string> _hs) {
		std::deque<token> nde;

		for (auto i = 0; i < _marc.body.size(); i++) {
			token t0 = _marc.body[i];
			token t1 = (i == _marc.body.size() - 1) ? token{} : _marc.body[i + 1];

			bool t0_pr = (t0.type == "mp");
			bool t1_pr = (t1.type == "mp") && (t1.type != "eof");

			//Treat the next token as original parameter
			if ((t0.value == "@") && (t1_pr)) {
				nde.push_back(_stringize(t0, std::deque<token>(1, _args[t1.arg_pos])));
				i++;
				continue;
			}

			//Stringize the next token as a pass parameter literal
			if ((t0.value == "@@") && (t1_pr)) {
				std::deque<token> arg(1, _args[t1.arg_pos]);

				if (t1.is_vararg && nde.size() > 0 && nde[nde.size() - 1].value == ",") {
					if (arg.size() > 0)
						nde.push_back(*arg.begin());
					else
						nde.pop_back();
				}
				else if (nde.size() > 0) {
					//Merge them, like if a##b then merge the next token to ab
					_glue_push(nde, arg[0]);
					
					for (auto& _a : arg) {
						nde.push_back(arg[i]);
					}
				}

				i++;
				continue;
			}

			if (t0.value == "@@" && t1.type != "eof") {
				_hs = t1.hidesets;
				_glue_push(nde, t1);
				i++;
				continue;
			}

			if (t0_pr && t1.type != "eof" && t1.value == "@@") {
				_hs = t1.hidesets;

				if (_args.size() <= t0.arg_pos) {
					i++;
				}
				else {
					nde.push_back(_args[t0.arg_pos]);
				}
				continue;
			}

			if (t0_pr) {
				
			}

			nde.push_back(t0);
		}

		return _add_hs(nde, _hs);
	}

	std::deque<token> spp::_read_one_arg(token tok, bool readall) {
		u32 lvl = 0;
		std::deque<token> nde{};

		for (;;) {
			token tok = lexer->peek_next();

			if (tok.type == "eof") {
				complier_error("Unterminated marco argument list");
			}
			else if (tok.type == "newline") {
				continue;
			}
			else if (tok.bol && tok.value == "$") {
				//_dir_par()
				continue;
			}
			else if ((tok.value == ")") && (lvl == 0)) {
				return nde;
			}
			else if ((tok.value == ",") && (lvl == 0) && !readall) {
				lexer->read_next();
				return nde;
			}
			else if ((tok.value == "("))
				lvl++;
			else if (tok.value == ")") lvl--;
	
			nde.push_back(tok);

			tok = lexer->read_next();

		}
	}

	std::deque<token> spp::_read_args(token tok, smacro marc) {
		lexer->get_next();

		if (marc.nargs == 0 && lexer->peek_next().value == ")")
			return std::deque<token>{};
		
		lexer->get_next();
		std::deque<token> nde;

		bool end = false;
		while (!end) {
			bool in_ellipsis = (marc.is_varg && marc.args.size() == marc.nargs - 1);
			std::deque<token> _hsde = _read_one_arg(tok, in_ellipsis);

			for (auto& _de : _hsde) {
				nde.push_back(_de);
			}
		}

		if (nde.size() != marc.nargs) {
			complier_error("Marco argument number does not match");
		};

		return nde;
	}

	void spp::_propagate(_tk_de _tkm, token tmpl) {
		//_tkm[0] = tmpl;
	}

	token spp::_expd_nl(token _tk) {
		if (_tk.type != "ident")
			return _tk;

		std::string name = _tk.value;
		smacro marc = macros[name];

		//If marco does not exists or is in the hideset
		if (marc.name == "" || _tk.hidesets.find(name) != _tk.hidesets.end()) {
			return _tk;
		}

		switch (marc.marco_kind) {
		case smacro::kind::object: {
			_tk.hidesets.emplace(name);
			std::deque<token> tks = _subst(marc, std::deque<token>{}, _tk.hidesets);

			lexer->add_pending_tokens<std::deque<token>>(tks);

			//_propagate(tks, _tk);
			return _expd(lexer->read_next());
		}

		case smacro::kind::function: {
			_tk.hidesets.emplace(name);
			std::deque<token> tks = _subst(marc, std::deque<token>{}, _tk.hidesets);

			return _expd(lexer->read_next());
		}
		case smacro::kind::special: {
			marc.special_marco_handler(_tk);

			return _expd(lexer->read_next());
		}
		default:
			complier_error("Unknown type of marco");

		}
	}

	s32 spp::_is_ppkw(token _tk) {
		for (u32 i = 0; i < PRPRWC; i++) {
			if (_tk.value == preprocess_keywords[i])
				return i;
		}

		return -1;
	}

	token spp::_expd(token _tk) {
		for (;;) {
			token tke = _expd_nl(_tk);

			if (tke.type != "newline")
				return tke;
		}
	}

	token spp::_expd_al(std::deque<token> _tks, token _tk) {
		return token{};
	}

	void spp::_obj_macr_par(token _tk) {
		smacro marc{};
		marc.name = _tk.value;

		for (;;) {
			token tok = lexer->read_next();

			if ((tok.type == "newline") || (tok.type == "eof")) {
				break;
			}

			marc.body.push_back(tok);
		}

		if ((marc.body[marc.body.size() - 1].value == "@@") || 
			marc.body[0].value == "@@")
		{
			complier_error("@@ can't be appeared at the beginning or the end of an marco!");
		}

		macros.insert(std::make_pair(_tk.value, marc));
	}

	token spp::_define_par(token _tk) {
		token name = lexer->read_next();

		if (name.type != "ident") {
			complier_error("Expected ident");
		}

		if (lexer->peek_next().value == "(") {
			lexer->read_next();

			_func_macr_par(name);

			return token{};
		}

		_obj_macr_par(name);

		return token{};
	}

	token spp::_dir_par(token _tk) {
		token _tk2 = lexer->read_next();
		s32 tkp = _is_ppkw(_tk2);

		if (tkp == -1) {
			return token{};
		}

		std::string kw = preprocess_keywords[tkp];

		if (kw == "def") {
			return _define_par(_tk2);
		}
		else if (kw == "undef") {
			return _undef_par(_tk2);
		}
	}

	token spp::_undef_par(token _tk) {
		token name = lexer->read_next();
		if (name.type != "ident") {
			complier_error("Ident expected, but " + name + " found");
		}
		lexer->expect("\n");
		macros.erase(name.value);

		return token{};
	}

	void spp::_func_macr_par(token _tk) {
		token_map _pmap;
		smacro _pmacr;

		_pmacr.name = _tk.value;

		_func_macr_params_par(_tk, _pmap);
		_pmacr.body = _func_macr_body_par(_pmap);

		for (auto m : _pmap) {
			_pmacr.args.push_back(m.second);
		}

		macros.insert(std::make_pair(_tk.value,_pmacr));
	}

	bool spp::_func_macr_params_par(token _tk, token_map& _params) {
		int pos = 0;

		for (;;) {
			token tok = lexer->read_next();

			token argz; argz.type = "marco";

			if (tok.value == ")")
				return false;

			if (pos) {
				if (tok.value != ",") complier_error(", expected but " + tok);
				tok = lexer->read_next(); 
			}

			if (tok.type == "newline")  {
				complier_error("Missing ')' in parameter list");
			}
			if (tok.value == "?~") {
				argz.arg_pos = pos++;
				argz.is_vararg = true;
				argz.value = "dynamic_arguments";

				_params.insert(std::make_pair("dynamic_arguments", argz));
				tok = lexer->get_next();
				lexer->expect(")");
				tok = lexer->get_next();
				return true;
			}
			if (tok.type != "ident") {
				complier_error("Identifier expected, but got " + tok.value);
			}
			std::string arg = tok.value;

			argz.arg_pos = pos++;
			argz.is_vararg = true;
			argz.value = arg;

			if (lexer->peek_next().value == "?~") {
				lexer->expect(")");

				lexer->read_next();

				_params.insert(std::make_pair(arg, argz));

				return true;
			}

			_params.insert(std::make_pair(arg, argz));

		}
	}

	spp::_bd_sco_mc spp::_func_macr_body_par(token_map& _params) {
		_bd_sco_mc b_sc;
		
		for (;;) {
			token tok = lexer->read_next();

			if ((tok.type == "newline") || (tok.type == "eof")){
				break;
			}
			
			if (tok.type == "ident") {
				auto res = _params.find(tok.value);

				if (res!= _params.end()) {
					//tok.value = (*res).second.value;
					b_sc.push_back(tok);

					continue;
				}
			}

			b_sc.push_back(tok);
		}

		return b_sc;
	}
}