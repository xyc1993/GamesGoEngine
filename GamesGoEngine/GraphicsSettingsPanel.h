#pragma once

#include "EditorPanel.h"

namespace GamesGoEngine
{
	class GraphicsSettingsPanel : public EditorPanel
	{
	public:
		GraphicsSettingsPanel();

		virtual void Draw() override;
	};
}