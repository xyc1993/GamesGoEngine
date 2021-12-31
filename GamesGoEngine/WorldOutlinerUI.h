#pragma once

#include "Scene.h"

class WorldOutlinerUI
{
public:
	static GameObject* Draw(Scene* activeScene);

private:
	static void DrawSceneNodeChildren(GameObject* sceneObject);

	static GameObject* selectedSceneObject;
};