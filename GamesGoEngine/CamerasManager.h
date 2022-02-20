#pragma once

#include <vector>

#include "Camera.h"

class CamerasManager
{
public:
	static glm::vec3 GetActiveCameraPosition();
	static glm::mat4 GetActiveCameraViewMatrix();
	static glm::mat4 GetActiveCameraProjectionMatrix();

	static void AddSceneCamera(Camera* camera);
	static void RemoveSceneCameras();
	static void RemoveSceneCamera(Camera* camera);
	static void SetCurrentViewAspectRatio(float aspectRatio);
	static float GetCurrentViewAspectRatio();

private:
	static bool CompareCameras(Camera* cam1, Camera* cam2);

	static std::vector<Camera*> sceneCameras;
	static float currentViewAspectRatio;
};