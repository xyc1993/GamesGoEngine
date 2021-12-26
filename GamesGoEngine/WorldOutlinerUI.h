#pragma once

#include "Scene.h"

class WorldOutlinerUI
{
public:
	static GameObject* Draw(const Scene& activeScene);

private:
	static void DrawSceneNodeChildren(GameObject* sceneObject);

	static constexpr float IndentSpace = 8.0f;
	static GameObject* selectedSceneObject;
};