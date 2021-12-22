#include "SceneExample.h"

SceneExample::SceneExample()
{
	scene = new Scene();
}

SceneExample::~SceneExample()
{
	delete scene;
}

void SceneExample::Update()
{
	scene->Update();
}

const Scene& SceneExample::GetScene() const
{
	return *scene;
}

Scene* SceneExample::GetSceneUnsafe() const
{
	return scene;
}