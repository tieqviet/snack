#include "stokenizer.h"

namespace snack {

	auto has_dot = false;

	bool is_keyword(std::string keywords[], std::string word) {
		for (u32 s = 0; s < KEYWORD_SIZE; s++) {
			if (keywords[s] == word) {
				return true;
			}
		}

		return false;
	}

	bool is_digit(s8 c) {
		std::string str(1, c); std::smatch m;

		return std::regex_search(str, m, std::regex("[0-9]"));
	}

	bool is_id_start(s8 c) {
		std::string str(1, c);
		std::smatch m;

		return std::regex_search(str, m, std::regex("[a-zA-Z_]"));
	}

	bool is_id(s8 c) {
		std::string str(1, c);
		std::smatch m;

		bool res = std::regex_search(str, m, std::regex("<>0123456789"));

		return is_id_start(c) || (m.position() >= 0 && res != false);
	}

	bool is_op_char(s8 c) {
		std::string str(1, c);
		std::smatch m;

		bool res = std::regex_search(str, m, std::regex("[$\"=%:*+-><=\'!|&?~]"));
	
		return res;

	}

	bool is_punc(s8 c) {

		std::string str(1, c);
		std::smatch m;

		bool res = std::regex_search(str, m, std::regex("[(),;{}]"));

		return res;

	}

	bool is_whitespace(s8 c) {
		std::string str(&c);
		std::smatch m;

		bool result = std::regex_search(str, m, std::regex(" (\r\n|\r|\n)"));

		int pos = m.position();
		return pos >= 0 && result != false;
	}

	bool is_newline(s8 c) {
		return c != '\n';
	}

	bool is_numbering_vaild(s8 c) {

		if (c == '.') {
			if (has_dot)
				return false;

			has_dot = true;

			return true;
		}

		return is_digit(c);
	}


	token::token(std::string type, std::string val) : type(type), value(val) {
	};
	token::token() {};

	bool token::operator == (token rhs) {
		return ((rhs.type == type) && (rhs.value == value));
	}

	tokenizer::tokenizer(spis& is, bool hidden_tok) : input(is), hidden_token(hidden_tok) {
			std::string keywordsz[KEYWORD_SIZE] = {
				"func",
				"lambda",
				"with",
				"int",
				"short",
				"word",
				"byte",
				"lword",
				"long"
				"qword",
				"qint"
				"bool",
				"real",
				"ext",
				"extext",
				"struct",
				"class",
				"public",
				"private",
				"protected"
				"bg",
				"end"
			};

			std::string typez[TYPE_SIZE] = {
				"int",
				"short",
				"word",
				"byte",
				"lword",
				"long"
				"qword",
				"qint"
				"bool",
				"real",
				"ext",
				"extext",
			};

			std::copy(std::begin(typez), std::end(typez), types);
			std::copy(std::begin(keywordsz), std::end(keywordsz), keywords);
		};

	tokenizer::tokenizer(): hidden_token(true) {
		std::string keywordsz[KEYWORD_SIZE] = {
			"func",
			"lambda",
			"with",
			"int",
			"short",
			"word",
			"byte",
			"lword",
			"long"
			"qword",
			"qint"
			"bool",
			"real",
			"ext",
			"extext",
			"struct",
			"class",
			"public",
			"private",
			"protected",
			"ret",
			"bg",
			"end"

		};

		std::string typez[TYPE_SIZE] = {
			"int",
			"short",
			"word",
			"byte",
			"lword",
			"long"
			"qword",
			"qint"
			"bool",
			"real",
			"ext",
			"extext",
		};

		std::copy(std::begin(typez), std::end(typez), types);
		std::copy(keywordsz, keywordsz + KEYWORD_SIZE, keywords);
	
	};

	bool tokenizer::is_optional_toks_hidden() {
		return hidden_token;
	}

	void tokenizer::optional_tokens_set_visibility(bool vis) {
		hidden_token = !vis;
	}

	std::string tokenizer::read_while(predicate_char_func func) {
			std::string str = "";
			while (!input.eof() && func(input.peek()))
				str += input.next();

			return str;
		}

	std::string tokenizer::read_escaped(s8 end) {
		auto escaped = false;
		std::string str;

		s8 c = input.next();

		while (!input.eof()) {
			c = input.next();

			if (escaped) {
				str += c;
				escaped = false;
			}
			else if (c == '\\') {
				escaped = true;
			}
			else if (c == end) {
				break;
			}
			else str += c;
		}

		return str;

	}

	spis& tokenizer::get_stream() {
		return input;
	}

	token tokenizer::read_string(s8 end) {
		return token("literal", read_escaped(end));
	}

	token tokenizer::read_number(u8 ch) {
		has_dot = false;

		return token("num", read_while(is_numbering_vaild));
	}

	token tokenizer::read_ident(u8 ch) {
		std::string id = read_while(is_id);

		return token(is_keyword(keywords, id) ? "kw" : "ident", id);
	}

	void tokenizer::skip() {
		std::string line = input.readline();
		//input.next();
	}

	void tokenizer::expect(std::string _pe) {
		if (peek_next().value != _pe) {
			complier_error("Got ? expected ?");
			return;
		}
	}

	token tokenizer::read_next() {

		u32 line_num = input.get_line_number();
		std::string unneed;

		if (input.get_line_number() > 1) {
			unneed = read_while(is_whitespace);
		}

		if (input.eof())
			return token("eof","-1");

		s8 ch = input.peek();

		if (ch <0) {
			return token("eof", "-1");
		}

		if (ch == '\n' || ch == '\t' || ch==' ') {
			if (!hidden_token) {
				if (ch == '\n')
				{
					ch = input.next();
					return token("newline", "\n");
				}
				else {
					while (ch == '\t'|| ch == ' ') {
						ch = input.next();
					}

					u32 pos = input.get_pos_number();

					input.set_pos(input.get_pos_number() - 1);
				}
					
			}
			else {
				while (ch == '\n' || ch == '\t'|| ch == ' ') {
					ch = input.next();
				}

				input.set_pos(input.get_pos_number() - 1);
			}
		}

		if (ch == '#') {
			skip();
			return read_next();
		}

		if ((ch == '"') || (std::string(1, ch) == "'")) {
			return read_string(ch);
		}

		if (is_digit(ch)) {
			return read_number(ch);
		}

		if (is_id_start(ch)) {
			return read_ident(ch);
		}

		if (is_punc(ch)) {
			ch = input.next();
			//Since we have only peeked, increase the pointer

			return token("ponc", std::string(1, ch));
		}

		if (is_op_char(ch)) {
			return token("op", read_while(is_op_char));
		}

		ch = input.next();
		return token("unidentified", std::string(1,ch));
	}

	token tokenizer::get_next() {
		return read_next();
	}

	token tokenizer::peek_next() {
		u64   _pos   = input.get_pos_number();
		token tok   = get_next();

		input.set_pos(_pos);

		return tok;
	}

	void tokenizer::reset_token_pointer() {
		token_pointer = 0;
	}


}