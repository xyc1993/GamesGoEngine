#pragma once

#include <string>

namespace GamesGoEngine
{
	class FileDialogs
	{
	public:
		// Opens file dialog that returns path of the opened file, empty string if cancelled
		static std::string OpenFile(const char* filter);
		// Opens file dialog for saving/creating files; returns saved/new file path, empty string if cancelled
		static std::string SaveFile(const char* filter);
	};
}