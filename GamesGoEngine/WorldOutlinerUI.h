#pragma once

#include "Scene.h"

class WorldOutlinerUI
{
public:
	static void Draw(const Scene& activeScene);

private:
	static int selectedSceneObjectIndex;
};