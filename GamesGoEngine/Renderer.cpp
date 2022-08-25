#include "Renderer.h"

#include "CamerasManager.h"
#include "GameObject.h"

Renderer::~Renderer()
{
	mesh.reset();
	CleanMaterialList();
}

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
				materialList[i]->Draw(owner->GetTransform()->GetTransformMatrix());
				mesh->DrawSubMesh(i);
			}
		}
	}
}

bool Renderer::TryGetMaterial(std::shared_ptr<Material>& outMaterial, int index)
{
	if (index >= 0 && index < materialList.size())
	{
		outMaterial = materialList[index];
		return true;
	}
	return false;
}

void Renderer::CleanMaterialList()
{
	for (size_t i = 0; i < materialList.size(); i++)
	{
		materialList[i].reset();
	}
	materialList.clear();
}

glm::vec3 Renderer::GetCameraPosition() const
{
	return CamerasManager::GetActiveCameraPosition();
}