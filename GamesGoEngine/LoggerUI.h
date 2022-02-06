#pragma once

#include <vector>

#include "imgui.h"
#include "Log.h"

class LoggerUI
{
public:
	static void Draw();
	static void ClearLogs();

private:
	static void AddMsg(MsgType type, std::string msg);
	static ImVec4 GetMsgColor(MsgType type);

	static std::vector<std::tuple<MsgType, std::string>> messages;

	friend class Log;
};