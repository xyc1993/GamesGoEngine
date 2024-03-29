#pragma once

#include <vector>
#include <imgui.h>

#include "EditorPanel.h"
#include "Log.h"

namespace GamesGoEngine
{
	class LoggerPanel : public EditorPanel
	{
	public:
		LoggerPanel();

		virtual void Draw() override;
		void ClearLogs();
		void AddLog(MsgType type, std::string msg);

	private:
		
		static ImVec4 GetMsgColor(MsgType type);

		std::vector<std::tuple<MsgType, std::string>> messages;
	};
}