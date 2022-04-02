#include "Renderer.h"

#include "CamerasManager.h"
#include "GameObject.h"

void Renderer::Update()
{

}

void Renderer::Draw()
{
	if (owner != nullptr && mesh != nullptr)
	{
		for (size_t i = 0; i < materialList.size(); i++)
		{
			if (materialList[i] != nullptr)
			{
				materialList[i]->Draw(owner->GetTransform()->GetTransformMatrix(), CamerasManager::GetActiveCameraViewMatrix(), CamerasManager::GetActiveCameraProjectionMatrix());
				mesh->DrawSubMesh(i);
			}
		}
	}
}

void Renderer::CleanMaterialList()
{
	for (size_t i = 0; i < materialList.size(); i++)
	{
		materialList[i].reset();
	}
	materialList.clear();
}