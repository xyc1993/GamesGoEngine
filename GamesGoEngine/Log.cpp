#include "Log.h"

#include <iostream>

#include "EditorUIManager.h"
#include "LoggerUI.h"

namespace GamesGoEngine
{
	void Log::PrintMsg(MsgType type, std::string msg)
	{
		const time_t currentTime = time(0);
		tm localTime;
		localtime_s(&localTime, &currentTime);

		char buffer[256];
		strftime(buffer, sizeof(buffer), "%Y.%b.%d %H:%M:%S", &localTime);

		const std::string logTimeString = buffer;
		const std::string logMsg = logTimeString + " " + msg;

		EditorUIManager::AddLog(type, logMsg);
	}

	void Log::ClearLogs()
	{
		EditorUIManager::ClearLogs();
	}
}