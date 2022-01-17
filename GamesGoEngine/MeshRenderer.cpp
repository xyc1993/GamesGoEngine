#include "MeshRenderer.h"

#include "CamerasManager.h"
#include "GameObject.h"

MeshRenderer::MeshRenderer()
{
	mesh = nullptr;
	materialList.clear();
}

MeshRenderer::~MeshRenderer()
{
	mesh.reset();
	CleanMaterialList();
}

void MeshRenderer::Update()
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

void MeshRenderer::SetMaterial(const std::shared_ptr<Material>& material)
{
	SetMaterial(material, 0);
}

void MeshRenderer::SetMaterial(const std::shared_ptr<Material>& material, size_t materialIndex)
{
	if (materialIndex >= 0 && materialIndex < materialList.size())
	{
		if (materialList[materialIndex] != material)
		{
			materialList[materialIndex].reset();
			materialList[materialIndex] = material;
		}
	}
}

void MeshRenderer::SetMesh(const std::shared_ptr<MeshBase>& mesh)
{
	if (this->mesh != mesh)
	{
		// firstly set mesh
		this->mesh.reset();
		this->mesh = mesh;

		// finally, do material cleanup & set proper size to the list
		CleanMaterialList();
		materialList.resize(this->mesh->GetSubMeshesCount());
	}	
}

size_t MeshRenderer::GetMaterialSlotsNumber() const
{
	if (mesh != nullptr)
	{
		return mesh->GetSubMeshesCount();
	}
	else
	{
		return 0;
	}
}

void MeshRenderer::CleanMaterialList()
{
	for (size_t i = 0; i < materialList.size(); i++)
	{
		materialList[i].reset();
	}
	materialList.clear();
}