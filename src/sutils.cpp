#include "sutils.h"

namespace snack {
	s8 read_char(std::stringstream& stream) {
		s8 cf; stream.read(&cf, sizeof(s8));

		return (cf);
	}

	s8 read_char(std::stringstream& stream, u32& pos) {
		if (pos != stream.tellg()) {
			stream.seekg(pos);
		}

		s8 c = read_char(stream);

		pos++;

		return c;
	}

	std::string trim(const std::string& str,
		const std::string& whitespace)
	{
		const auto strBegin = str.find_first_not_of(whitespace);
		if (strBegin == std::string::npos)
			return ""; // no content

		const auto strEnd = str.find_last_not_of(whitespace);
		const auto strRange = strEnd - strBegin + 1;

		return str.substr(strBegin, strRange);
	}

}