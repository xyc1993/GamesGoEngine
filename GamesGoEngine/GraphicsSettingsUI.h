#pragma once

#include "EditorPanel.h"

namespace GamesGoEngine
{
	class GraphicsSettingsUI : public EditorPanel
	{
	public:
		GraphicsSettingsUI();

		virtual void Draw() override;
	};
}