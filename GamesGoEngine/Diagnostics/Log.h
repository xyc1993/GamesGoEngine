#pragma once

#ifdef GAMES_GO_ENGINE_EXPORTS
#define GAMES_GO_ENGINE_API __declspec(dllexport)
#else
#define GAMES_GO_ENGINE_API __declspec(dllimport)
#endif

namespace GamesGoEngine
{
	class GAMES_GO_ENGINE_API Log
	{
	public:
		static void Info(const char* message);
	};
}