#pragma once

#include <string>

#include "sttype.h"
#include "stokenizer.h"

#define PRPRWC 7

namespace snack {

	const std::string preprocess_keywords[PRPRWC] = {
		"def",
		"undef"
		"ndef",
		"not",
		"if",
		"else",
		"elif",
		"prag"
	};

	int compare(std::string lhs, std::string rhs);

	struct smarco {
		std::string name;

		std::vector<std::string> template_parameters;
		std::string main_marco;
	};

	class spreprocessor {
		
		std::vector<smarco> defined_marcos;
		tokenizer lexer;

		bool _is_preprocess_word(std::string p_val) {
			return search<std::string>(std::vector<std::string>(std::begin(preprocess_keywords), std::end(preprocess_keywords)), p_val, compare_func<std::string>(&compare)) != -1;
		}

		bool _is_pr_line();

		s32 _preprocess_word_pos(s32 bg, s32 end, std::string p_val);

		std::vector<std::string> _parse_marco_paras();
		std::vector<std::string> _parse_marco_paste_paras();
		std::string				 _parse_mar_cont();
		bool					 _parse_pre_def_statement();

		bool _parse_pre_undef_statement();

		s32 _find_marcos(token tok);
		s32 _find_marcos_pos(s32 bg, s32 end, token tok);

		bool _paste_marco(smarco mar);
		bool _parse_pre_incl_statement();

		token read_next();

	public:

		spreprocessor() = default;
		spreprocessor(tokenizer& p_lexer);

		bool process();

		spis& get_stream();
	};
}