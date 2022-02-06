#pragma once

#include <string>

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
};