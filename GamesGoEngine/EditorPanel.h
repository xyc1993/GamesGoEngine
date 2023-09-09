#pragma once

namespace GamesGoEngine
{
	class EditorPanel
	{
	public:
		EditorPanel();

		virtual void Draw() = 0;
		// Draws panel and sets size and position
		void DrawToWindowDimensions(float windowWidth, float windowHeight);
		// Sets multipliers used to determine panel position and offset
		void SetDrawingMultipliers(float posWidthMul, float posHeightMul, float sizeWidthMul, float sizeHeightMul);

	protected:
		// Used to calculate position offset, offset is multiplier times window dimension (width/height)
		float posWidthMultiplier;
		float posHeightMultiplier;
		// Used to calculate size, size is multiplier times window dimension (width/height)
		float sizeWidthMultiplier;
		float sizeHeightMultiplier;
	};
}