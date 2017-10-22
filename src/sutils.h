#pragma once

/*PRECOMPILED INCLUDING*/
#include <fstream>
#include <functional>
#include <vector>

/*SOURCE INCLUDING*/
#include "spis.h"

template <typename N>
using compare_func = std::function<int(N, N)>;

namespace snack {
	s8 read_char(std::stringstream& stream);
	s8 read_char(std::stringstream& stream, u32& pos);

	std::string trim(const std::string& str,
		const std::string& whitespace = " ");

	template <typename T>
	s64 search(std::vector<T>& arr, const T& val, compare_func<T> comp_func) {
		if (arr.size() > 1) {
			if (arr.size() <= 8) {
				s8 res;

				for (u32 i = 0; i < arr.size(); i++) {
					res = (comp_func(arr[i], val));

					if (res == 0)
						return i;
				}

				return -1;
			}

			return binary_search_op(arr, 0, arr.size() - 1, val, comp_func);
		}

		return arr.size() - 1;
	}

	template <typename T>
	s64 binary_search_op(std::vector<T>& arr, s64 bg, s64 end, const T& p_val, compare_func<T> comp_func) {
		if (bg > end)
			return -1;

		int mid = (bg + end) / 2;

		if (comp_func(arr[mid], p_val) == 0)
			return mid;
		else if (comp_func(arr[mid], p_val) == -1)
			return binary_search_op(arr, mid+1, end, p_val, comp_func);

		return  binary_search_op(arr, bg, mid-1, p_val, comp_func);

	}
}