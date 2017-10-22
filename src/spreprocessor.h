#pragma once

#include <string>

#include "sttype.h"
#include "stokenizer.h"

#define PRPRWC 7

namespace snack {

	std::string preprocess_keywords[] = {
		"def",
		"undef"
		"ndef",
		"not",
		"if",
		"else",
		"elif",
		"prag"
	};

	struct smarco {
		std::string name;

		std::vector<std::string> template_parameters;
		std::string main_marco;
	};

	struct spreprocessor {
		
		spreprocessor(tokenizer& p_lexer) : lexer(p_lexer) {
			lexer.optional_tokens_set_visibility(false);
		}

		std::vector<smarco> defined_marcos;

		tokenizer lexer;

		bool _is_preprocess_word(std::string p_val) {
			return _preprocess_word_pos(0, PRPRWC - 1, p_val) != -1;
		}

		u32 _preprocess_word_pos(u32 bg, u32 end, std::string p_val) {
			/*
			if (end >= bg) {
				int mid = bg + (end - bg) / 2;

				if (preprocess_keywords[mid] == p_val)
					return mid;
				else if (preprocess_keywords[mid] < p_val)
					return _preprocess_word_pos(bg, mid++, p_val);
				else if (preprocess_keywords[mid] > p_val)
					return _preprocess_word_pos(mid--, end, p_val);
			}

			return -1;*/

			for (u32 i = bg; i <= end; i++) {
				if (preprocess_keywords[i] == p_val)
					return i;
			}

			return -1;
		}

		bool _is_pr_line() {
			return lexer.peek_next().value == "$";
		}

		std::vector<std::string> _parse_marco_paras() {
			std::vector<std::string> paras;

			bool parentheses = true;
			bool identified = false;

			token tok;
			
			tok = lexer.peek_next();

			if (tok.value == "(") {

				tok = lexer.get_next();

				while (tok.value != ")") {
					tok = lexer.get_next();

					if (tok.type != "var") {
						if (tok.value == ",") {
							if (!identified) {
								complier_error("Expected an identifier");
							}
							else
								identified = false;
						}
						else if (tok.value != ")") {
							complier_error("Unexpected identifier");
						}
					}
					else {
						if (!identified) {
							paras.push_back(tok.value);

							identified = true;
						}
						else {
							complier_error("Expected )");
						}
					}
				}
			}

			return paras;
		}

		std::string _parse_mar_cont() {
			std::string marco_cont;

			bool null_terminited = false;

			s8 c = lexer.get_stream().peek();

			for (;;) {
				c = lexer.get_stream().next();

				if (c == '\\') {
					null_terminited = true;
					//delete characters
				}
				else {
					if ((null_terminited) && (c == '\n')) {
						null_terminited = false;
					}
					else if (!null_terminited)  {
						if (c == '\n')
							break;
						else if (c != '\\')
							marco_cont += c;
						//delete characters
					}
				}
			}

			marco_cont = trim(marco_cont);

			return marco_cont;
		}

		bool _parse_pre_def_statement() {
			u32 pos = lexer.get_pos_num();
			token tok = lexer.get_next();

			if (tok.type == "new_line") {
				complier_error("Expected an identifier");
			}
			else if (tok.type != "var") {
				complier_error("Identifier invalid")
			}

			smarco marco;

			marco.name = tok.value;

			tok = lexer.peek_next();

			marco.template_parameters = _parse_marco_paras();
			
			marco.main_marco = _parse_mar_cont();

			defined_marcos.push_back(marco);

			u32 pos_del = lexer.get_pos_num();

			lexer.get_stream().erase(pos - 4, pos_del - 1);

			return true;
		}

		spis& get_stream() {
			return lexer.get_stream();
		}

		bool _parse_pre_undef_statement() {
			u32 pos = lexer.get_pos_num();
			token tok = lexer.get_next();

			if (tok.type == "new_line") {
				complier_error("Expected an identifier");
				return false;
			}

			for (u32 i = 0; i < defined_marcos.size(); i++) {
				if (defined_marcos[i].name == tok.value) {
					defined_marcos.erase(std::begin(defined_marcos) + i);
				}
			}

			lexer.get_stream().readline();
			lexer.get_stream().erase(pos - 6, lexer.get_pos_num() );

			return true;
		}

		s32 _find_marcos(token tok) {
			for (u32 i = 0; i < defined_marcos.size(); i++) {
				if (defined_marcos[i].name == tok.value) {
					return i;
				}
			}

			return -1;
		}

		bool _paste_marco(smarco mar) {

			u32 pos = lexer.get_pos_num() - mar.name.size();

			std::vector<std::string> paras = _parse_marco_paras();
			std::stringstream strstream(mar.main_marco);

			std::string marco_cont = mar.main_marco;

			spis stream(&strstream);
			tokenizer mar_lexer(stream);

			token tok;

			if (paras.size() != 0) {
				while (tok.type != "eof") {
					tok = mar_lexer.get_next();

					if (tok.type == "var") {
						for (u32 i = 0; i < mar.template_parameters.size(); i++) {
							if (tok.value == mar.template_parameters[i]) {
								//if the parameters is same, skip it
								if (mar.template_parameters[i] != paras[i]) {
									marco_cont.erase(mar_lexer.get_pos_num() - 1, mar.template_parameters[i].size());
									marco_cont.insert(mar_lexer.get_pos_num() - 1, paras[i]);
								}
							}
						}
					}
				}
			}

			//lexer.get_stream().readline();

			lexer.get_stream().erase(pos, lexer.get_pos_num() - 1);
			lexer.get_stream().write(marco_cont,  pos);

			return true;
		}

		bool process() {
			token tok;

			while (tok.type != "eof") {

				if (_is_pr_line()) {
					tok = lexer.get_next();
					tok = lexer.get_next();

					if (_is_preprocess_word(tok.value) ==-1) {
						complier_error("Unidentified keyword after $");
					}
					else {
						std::string s = tok.value;

						if (tok.value == "def") {
							_parse_pre_def_statement();
						}
						else if (tok.value == "undef") {
							_parse_pre_undef_statement();
						}
						else if (tok.value == "prag") {
							//For now, we skip
							//lexer.skip();
							//Prag is now jus
						}
					}
				}
				else {
					//FIND MARCO
					tok = lexer.get_next();

					while ((tok.type != "new_line")){
						
						if (tok.type == "eof")
							break;
						else {
							s32 pos = _find_marcos(tok);

							if (pos != -1) {
								_paste_marco(defined_marcos[pos]);
							}
						}

						tok = lexer.get_next();

					}

					//lexer.skip();
				}

			}
			return true;
		}

	};
}