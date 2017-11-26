#pragma once

#include <fstream>
#include <vector>
#include <regex>

#include "spis.h"
#include "stype.h"

#define KEYWORD_SIZE 30
#define TYPE_SIZE 30

typedef bool(*predicate_char_func)(s8 ch);
typedef bool(*predicate_keyword_func)(std::string[], std::string);

namespace snack {

	bool is_keyword(std::string keywords[], std::string word);
	bool is_digit(s8 c);
	bool is_id_start(s8 c);
	bool is_id(s8 c);
	bool is_op_char(s8 c);
	bool is_punc(s8 c);
	bool is_whitespace(s8 c);
	bool is_newline(s8 c);
	bool is_numbering_vaild(s8 c);

	class tokenizer {

		std::vector<token> tokens;

		u32 token_pointer;

		spis input;
	
		std::string read_while(predicate_char_func func);
		std::string read_escaped(s8 end);
		token read_string(s8 end);
		token read_number(u8 ch);
		token read_ident(u8 ch);

		bool hidden_token;

	public:

		void skip();

		u32 get_line_num() {
			return input.get_line_number();
		}

		u32 get_col_num() {
			return input.get_col_number();
		}

		u32 get_pos_num() {
			return input.get_pos_number();
		}

		token get_next();
		token read_next();
		token peek_next();

		//token read_once(std::string _v);

		void expect(std::string _pe);

		spis& get_stream();

		void reset_token_pointer();

		tokenizer(spis& is, bool hidden_tok = true);
		tokenizer();

		bool is_optional_toks_hidden();
		void optional_tokens_set_visibility(bool vis);

		std::string keywords[KEYWORD_SIZE];
		std::string types[TYPE_SIZE];

	};

}
