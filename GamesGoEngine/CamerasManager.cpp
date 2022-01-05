#include "CamerasManager.h"

#include <algorithm>

std::vector<Camera*> CamerasManager::sceneCameras;

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

bool CamerasManager::CompareCameras(Camera* cam1, Camera* cam2)
{
	return cam1->GetCameraPriority() > cam2->GetCameraPriority();
}