#pragma once

#include "Scene.h"

class SceneExample
{
public:
	SceneExample();
	~SceneExample();
	
	virtual void Update();
	void Clear() const;

protected:
	Scene* scene;
};