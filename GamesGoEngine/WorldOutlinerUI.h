#pragma once

#include "Scene.h"

class WorldOutlinerUI
{
public:
	static int Draw(const Scene& activeScene);

private:
	static int selectedSceneObjectIndex;
};