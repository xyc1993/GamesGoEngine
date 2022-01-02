#pragma once

#include "GameObject.h"

class PropertiesUI
{
public:
	static void Draw(GameObject* selectedGameObject);

private:
	static void DrawNameInputField(GameObject* selectedGameObject);
	static void DrawTransformField(GameObject* selectedGameObject);
};