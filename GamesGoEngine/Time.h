#pragma once

namespace GamesGoEngine
{
	class Time
	{
	private:
		Time();

		static Time* instance;
		static Time* GetInstance();

	public:
		static double GetTime();
		static double GetDeltaTime();
		static double GetUnscaledTime();
		static double GetUnscaledDeltaTime();
		static double GetTimeScale();

		static void SetTimeScale(const double scale);

		static void Update();

	private:
		double time;
		double deltaTime;
		double unscaledTime;
		double unscaledDeltaTime;
		double timeScale;

		double currentFrameTime;
		double lastFrameTime;
	};
}