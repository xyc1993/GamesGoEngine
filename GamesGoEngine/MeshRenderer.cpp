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
	mesh->DeleteSafely();
	for (size_t i = 0; i < materialList.size(); i++)
	{
		if (materialList[i] != nullptr)
		{
			materialList[i]->DeleteSafely();
		}
	}
}

void MeshRenderer::Init(GameObject* owner)
{
	Component::Init(owner);
	
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

void MeshRenderer::SetMaterial(Material* material)
{
	SetMaterial(material, 0);
}

void MeshRenderer::SetMaterial(Material* material, size_t materialIndex)
{
	if (materialIndex < 0 || materialIndex >= materialList.size())
	{
		return;
	}
	
	// firstly, if there's some material assigned, decrement its users number
	if (materialList[materialIndex] != nullptr)
	{
		materialList[materialIndex]->DecrementNumberOfUsers();
	}

	// secondly assign new material at given index & increment the number of users for the new material
	materialList[materialIndex] = material;
	materialList[materialIndex]->IncrementNumberOfUsers();
}

void MeshRenderer::SetMesh(MeshBase* mesh)
{
	if (this->mesh == mesh)
	{
		return;
	}
	
	// firstly, if there's some mesh assigned, decrement its users number
	if (this->mesh != nullptr)
	{
		this->mesh->DecrementNumberOfUsers();
	}
	
	// secondly assign new mesh & increment the number of users for the new mesh
	this->mesh = mesh;
	this->mesh->IncrementNumberOfUsers();
	
	// finally, do material cleanup & set proper size to the list
	CleanMaterialList();
	materialList.resize(this->mesh->GetSubMeshesCount());
}

void MeshRenderer::CleanMaterialList()
{
	for (size_t i = 0; i < materialList.size(); i++)
	{
		if (materialList[i] != nullptr)
		{
			materialList[i]->DecrementNumberOfUsers();
			// unreference material instead of destroying it
			materialList[i] = nullptr;
		}
	}

	// don't do this, it'll call destructor of materials, & we don't necessarily want that
	//materialList.clear();
}