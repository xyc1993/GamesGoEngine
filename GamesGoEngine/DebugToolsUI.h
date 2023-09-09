#pragma once

#include "EditorPanel.h"

namespace GamesGoEngine
{
	class DebugToolsUI : public EditorPanel
	{
	public:
		DebugToolsUI();

		virtual void Draw() override;
	};
}