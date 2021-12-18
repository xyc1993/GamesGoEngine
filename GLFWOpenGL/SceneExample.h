#pragma once

#include "Scene.h"

class SceneExample
{
public:
	SceneExample();
	~SceneExample();
	
	virtual void Update();

protected:
	Scene* scene;
};