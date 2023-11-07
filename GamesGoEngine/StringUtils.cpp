#include "StringUtils.h"

#include <sstream>

namespace GamesGoEngine
{
	unsigned int StringUtils::GetWordsCount(const std::string& checkedString)
	{
        std::istringstream iss(checkedString); // Create a string stream from the input string
        unsigned int wordCount = 0;
        std::string word;

        while (iss >> word) {
            wordCount++;
        }

        return wordCount;
	}

    bool StringUtils::IsWordsCountEqual(const std::string& checkedString, unsigned int checkedWordCount)
	{
        std::istringstream iss(checkedString); // Create a string stream from the input string
        unsigned int wordCount = 0;
        std::string word;

        while (iss >> word)
        {
            wordCount++;
            if (wordCount > checkedWordCount)
            {
                return false;
            }
            if (wordCount == checkedWordCount)
            {
                return true;
            }
        }

        return false;
	}
}