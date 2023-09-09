#pragma once

#include "EditorPanel.h"

namespace GamesGoEngine
{
	class DebugToolsUI : EditorPanel
	{
	public:
		DebugToolsUI();

		virtual void Draw() override;
	};
}