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