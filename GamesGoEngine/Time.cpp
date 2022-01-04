#include "Time.h"

#include <GLFW/glfw3.h>

Time* Time::instance = new Time();

Time::Time()
{
	time = 0.0;
	deltaTime = 0.0;
	unscaledTime = 0.0;
	unscaledDeltaTime = 0.0;
	timeScale = 1.0;
	currentFrameTime = 0.0;
	lastFrameTime = 0.0;
}

Time* Time::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Time();
	}
	return instance;
}

double Time::GetTime()
{
	return GetInstance()->time;
}

double Time::GetDeltaTime()
{
	return GetInstance()->deltaTime;
}

double Time::GetUnscaledTime()
{
	return GetInstance()->unscaledTime;
}

double Time::GetUnscaledDeltaTime()
{
	return GetInstance()->unscaledDeltaTime;
}

double Time::GetTimeScale()
{
	return GetInstance()->timeScale;
}

void Time::SetTimeScale(const double scale)
{
	GetInstance()->timeScale = scale;
}

void Time::Update()
{
	const double currentFrame = glfwGetTime();

	GetInstance()->currentFrameTime = currentFrame;

	GetInstance()->unscaledDeltaTime = GetInstance()->currentFrameTime - GetInstance()->lastFrameTime;
	GetInstance()->deltaTime = GetTimeScale() * GetUnscaledDeltaTime();
	
	GetInstance()->unscaledTime += GetUnscaledDeltaTime();
	GetInstance()->time += GetDeltaTime();

	GetInstance()->lastFrameTime = currentFrame;
}