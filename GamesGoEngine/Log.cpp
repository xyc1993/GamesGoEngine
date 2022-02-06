#include "Log.h"

#include <iostream>

#include "LoggerUI.h"

void Log::PrintMsg(MsgType type, std::string msg)
{
	const time_t currentTime = time(0);
	tm localTime;
	localtime_s(&localTime , &currentTime);

	char buffer[256];
	strftime(buffer, sizeof(buffer), "%Y.%b.%d %H:%M:%S", &localTime);

	const std::string logTimeString = buffer;
	const std::string logMsg = logTimeString + " " + msg;

	LoggerUI::AddMsg(type, logMsg);
}