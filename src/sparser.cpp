#include "sparser.h"
#include "stfunc.h"

namespace snack {

	sparser::sparser(tokenizer& p_tokenizer) : lexer(p_tokenizer) {};

	stfunc* _parse_func() {
		stfunc* func;

		return func;
	}

	bool sparser::_is_keyword(token tok) {
		return tok.type == "kw";
	}

	bool sparser::_is_builtintype(token tok) {
		if (tok.type != "kw")  return false;

		for (auto t : lexer.types) {
			if (tok.value == t) {
				return true;
			}
		}

		return false;
	}

	bool sparser::_is_definedtype(token tok, sttype* dtp) {
		if (tok.type == "kw")  return false;

		for (auto t: defined_types) {
			if (tok.value == t->name) {
				if (dtp != nullptr)	
					dtp = t;
				
				return true;
			}
		}

		return false;
	}

	bool sparser::_is_type(token tok) {
		return _is_definedtype(tok, nullptr) || _is_builtintype(tok);
	}

	bool sparser::_is_op(token tok) {
		return tok.type == "op";
	}

	bool sparser::_is_var(token tok) {
		return tok.type == "var";
	}

	bool sparser::_is_pun(token tok) {
		return tok.type == "pun";
	}

	sttype* sparser::_parse_var_type() {
		token tok = lexer.get_next();
		std::string type_name;

		if (tok.type == "kw") {
			for (auto bt : lexer.types) {
				if (bt == tok.value) {
					type_name = tok.value;
				}
			}

			stbuiltin* ptype;

			ptype->name = type_name;

			if (type_name == "byte") {
				ptype->type = byte_type;
			}
			else if (type_name == "char") {
				ptype->type = char_type;
			}
			else if (type_name == "word") {
				ptype->type = word_type;
			}
			else if (type_name == "short") {
				ptype->type = short_type;
			}
			else if (type_name == "int") {
				ptype->type = int_type;
			}
			else if (type_name == "lint") {
				ptype->type = lint_type;
			}
			else if (type_name == "lword") {
				ptype->type = lword_type;
			}
			else if (type_name == "qint") {
				ptype->type = qint_type;
			}
			else if (type_name == "qword") {
				ptype->type = qword_type;
			}
			else if (type_name == "bool") {
				ptype->type = bool_type;
			}
			else if (type_name == "real") {
				ptype->type = real_type;
			}
			else if (type_name == "ext") {
				ptype->type = ext_type;
			}
			else if (type_name == "extext") {
				ptype->type = extext_type;
			}

			token next_tok = lexer.peek_next();

			if (next_tok.type == "op") {
				if (next_tok.value == "*") {
					ptype->is_pointer = true;
				}
				else if (next_tok.value == "=") {
					complier_error("Unexpected operator. Expected assign or pointer operator.");

					return nullptr;
				}
			}

			return (sttype*)(ptype);
		}
		else {
			for (auto defined_type : defined_types) {
				if (defined_type->name == tok.value) {
					return defined_type;
				}
			}
		}

		complier_error("Type specified not found.");

		return nullptr;

	}

	stvar* sparser::_parse_var() {
		stvar* var;

		token tok = lexer.get_next();

		var->set_name(tok.value);

		tok = lexer.get_next();

		if ((tok.type != "kw") || (tok.value == "as")) {
			complier_error("Type identifier expected");

			return nullptr;
		}

		var->base_type = _parse_var_type();

		return var;
	}

	sttype* sparser::_parse_define_statement() {
		sttype* base_type = _parse_var_type();

		token tok = lexer.get_next();

		if ((tok.type != "kw") || (tok.value == "as")) {
			complier_error("Type identifier expected");

			return nullptr;
		}

		tok = lexer.get_next();

		if (tok.type != "var") {
			complier_error("Defined type is not valid");
			
			return nullptr;
		}
		
		if (!(_is_type(tok))) {
			base_type->name = tok.value;
			
			return base_type;
		}
	}
	

}