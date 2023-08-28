#pragma once

namespace GamesGoEngine
{
	class Math
	{
	public:
		static bool IsNearlyZero(float value);
		static bool IsNearlyZero(float value, float errorMargin);

	private:
		static const float SMALL_NUMBER;
	};
}