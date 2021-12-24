#pragma once

#include "Scene.h"

class WorldOutlinerUI
{
public:
	static int Draw(const Scene& activeScene);

private:
	static constexpr float IndentSpace = 8.0f;
	static int selectedSceneObjectIndex;
};