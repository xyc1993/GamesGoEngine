#pragma once

#include <vector>

#include "Camera.h"

class CamerasManager
{
public:
	static glm::mat4 GetActiveCameraViewMatrix();
	static void AddSceneCamera(Camera* camera);
	static void RemoveSceneCameras();
	static void RemoveSceneCamera(Camera* camera);

private:
	static bool CompareCameras(Camera* cam1, Camera* cam2);

	static std::vector<Camera*> sceneCameras;
};