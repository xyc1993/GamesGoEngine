#pragma once

#include <string>

namespace GamesGoEngine
{
	enum class MsgType
	{
		Info,
		Warning,
		Error
	};

	class Log
	{
	public:
		static void PrintMsg(MsgType type, std::string msg);
		static void ClearLogs();
	};
}