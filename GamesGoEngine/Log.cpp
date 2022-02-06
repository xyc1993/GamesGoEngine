#include "Log.h"

#include <iostream>

#include "LoggerUI.h"

void Log::PrintMsg(MsgType type, std::string msg)
{
	const time_t ttime = time(0);
	tm local_time;
	localtime_s(&local_time , &ttime);

	char buffer[256];
	strftime(buffer, sizeof(buffer), "%Y.%b.%d %H:%M:%S", &local_time);

	const std::string logTimeString = buffer;
	const std::string logMsg = logTimeString + " " + msg;

	LoggerUI::AddMsg(type, logMsg);
}