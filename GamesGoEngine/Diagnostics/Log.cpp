#include "Log.h"

#include <cstdio>

namespace GamesGoEngine
{
	void Log::Info(const char* message)
	{
		std::printf("%s\n", message);
	}
}