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

void SceneExample::Clear() const
{
	scene->Clear();
}