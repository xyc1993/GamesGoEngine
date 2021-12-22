#pragma once

#include "Scene.h"

class SceneExample
{
public:
	SceneExample();
	~SceneExample();
	
	virtual void Update();
	const Scene& GetScene() const;
	Scene* GetSceneUnsafe() const;

protected:
	Scene* scene;
};