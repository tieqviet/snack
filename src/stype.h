#pragma once

#include <string>
#include <set>

#define complier_error(x)

typedef unsigned int u32;
typedef int s32;

typedef unsigned char u8;
typedef char s8;

typedef unsigned short u16;
typedef short s16;

typedef unsigned long u64;
typedef long s64;

typedef unsigned long long usize;
typedef long long ssize;

namespace snack {
	struct token {
		std::string type = "unassigned";
		std::string value = "unassigned";

		std::set<std::string> hidesets;

		token(std::string type, std::string val);
		token();

		bool operator == (token rhs);

		u32 id = -1;

		bool is_vararg = false;

		bool bol = false;
		bool is_marco_param = false;

		u32 arg_pos = -1;
			
	};
}

