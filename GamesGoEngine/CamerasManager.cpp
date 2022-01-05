#include "CamerasManager.h"

#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>

std::vector<Camera*> CamerasManager::sceneCameras;
float CamerasManager::currentViewAspectRatio = 16.0f / 9.0f;

glm::mat4 CamerasManager::GetActiveCameraViewMatrix()
{
	if (sceneCameras.empty())
	{
		return glm::mat4(1.0f);
	}
	else
	{
		// always return view matrix of camera with highest priority
		return sceneCameras[0]->GetCurrentViewMatrix();
	}
}

glm::mat4 CamerasManager::GetActiveCameraProjectionMatrix()
{
	if (sceneCameras.empty())
	{
		return glm::perspective(45.0f, currentViewAspectRatio, 0.1f, 1000.0f);
	}
	else
	{
		// always return projection matrix of camera with highest priority
		return sceneCameras[0]->GetCurrentProjectionMatrix();
	}
}

void CamerasManager::AddSceneCamera(Camera* camera)
{
	sceneCameras.push_back(camera);
	std::sort(sceneCameras.begin(), sceneCameras.end(), CompareCameras);
}

void CamerasManager::RemoveSceneCameras()
{
	// for now just clear all of the cameras in the container
	// TODO: when more advanced scene management is created remove only deleted scene cameras
	sceneCameras.clear();
}

void CamerasManager::RemoveSceneCamera(Camera* camera)
{
	for (size_t i = 0; i < sceneCameras.size(); i++)
	{
		if (sceneCameras[i] == camera)
		{
			sceneCameras.erase(sceneCameras.begin() + i);
			break;
		}
	}
}

void CamerasManager::SetCurrentViewAspectRatio(float aspectRatio)
{
	currentViewAspectRatio = aspectRatio;
}

float CamerasManager::GetCurrentViewAspectRatio()
{
	return currentViewAspectRatio;
}

bool CamerasManager::CompareCameras(Camera* cam1, Camera* cam2)
{
	return cam1->GetCameraPriority() > cam2->GetCameraPriority();
}