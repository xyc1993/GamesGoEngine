#pragma once
#include "EditorPanel.h"

namespace GamesGoEngine
{
	class EditorSettingsPanel : public EditorPanel
	{
	public:
		virtual void Draw() override;

	private:
		float fontGlobalScale = 1.0f;
	};
}