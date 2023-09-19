#pragma once

#include "EditorPanel.h"

namespace GamesGoEngine
{
	class DebugToolsPanel : public EditorPanel
	{
	public:
		DebugToolsPanel();

		virtual void Draw() override;
	};
}