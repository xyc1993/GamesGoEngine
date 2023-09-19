#include "LoggerUI.h"

#include <tuple>

namespace GamesGoEngine
{
	LoggerUI::LoggerUI()
	{
		messages.clear();
	}

	void LoggerUI::Draw()
	{
		ImGui::Begin("Log");

		ImGui::BeginChild("Buttons", ImVec2(0, 40));
		ImGui::Button("Clear logs");
		if (ImGui::IsItemClicked())
		{
			ClearLogs();
		}
		ImGui::EndChild();

		ImGui::BeginChild("Messages");
		for (size_t i = 0; i < messages.size(); i++)
		{
			MsgType type = std::get<0>(messages[i]);
			std::string msg = std::get<1>(messages[i]);
			ImGui::TextColored(GetMsgColor(type), msg.c_str());
		}

		const float scrollDifference = ImGui::GetScrollMaxY() - ImGui::GetScrollY();
		constexpr float maxScrollDifference = 40.0f;
		if (scrollDifference <= maxScrollDifference)
		{
			ImGui::SetScrollY(ImGui::GetScrollMaxY());
		}

		ImGui::EndChild();

		ImGui::End();
	}

	void LoggerUI::ClearLogs()
	{
		messages.clear();
	}

	void LoggerUI::AddLog(MsgType type, std::string msg)
	{
		const std::tuple<MsgType, std::string> logMsg = std::make_tuple(type, msg);
		messages.push_back(logMsg);
	}

	ImVec4 LoggerUI::GetMsgColor(MsgType type)
	{
		switch (type)
		{
		case MsgType::Info:
			return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		case MsgType::Warning:
			return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
		case MsgType::Error:
			return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
			//just in case someone adds another type of message but won't update this method
		default:
			return ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
		}
	}
}