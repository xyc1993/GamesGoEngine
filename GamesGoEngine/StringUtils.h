#pragma once

#include <string>

namespace GamesGoEngine
{
	class StringUtils
	{
	public:
		static unsigned int GetWordsCount(const std::string& checkedString);
		static bool IsWordsCountEqual(const std::string& checkedString, unsigned int checkedWordCount);
	};
}