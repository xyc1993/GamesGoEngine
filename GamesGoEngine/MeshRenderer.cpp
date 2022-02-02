#include "MeshRenderer.h"

#include "CamerasManager.h"
#include "GameObject.h"

MeshRenderer::MeshRenderer()
{
	mesh = nullptr;
	materialList.clear();

	editorOutlineMaterial = new Material("res/shaders/unlit.vert.glsl", "res/shaders/unlit.frag.glsl");
	editorOutlineMaterial->SetVector3((GLchar*)"unlitColor", glm::vec3(1.0f, 0.8f, 0.0f));
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
		if (owner->IsSelected())
		{
			return;
		}

		for (size_t i = 0; i < materialList.size(); i++)
		{
			if (materialList[i] != nullptr)
			{
				glStencilMask(0x00);
				materialList[i]->Draw(owner->GetTransform()->GetTransformMatrix(), CamerasManager::GetActiveCameraViewMatrix(), CamerasManager::GetActiveCameraProjectionMatrix());
				mesh->DrawSubMesh(i);
			}
		}
	}
}

void MeshRenderer::LateUpdate()
{
	if (owner != nullptr && mesh != nullptr)
	{
		if (!owner->IsSelected())
		{
			return;
		}

		for (size_t i = 0; i < materialList.size(); i++)
		{
			if (materialList[i] != nullptr)
			{
				// draw the outline for the selected game object
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);
				materialList[i]->Draw(owner->GetTransform()->GetTransformMatrix(), CamerasManager::GetActiveCameraViewMatrix(), CamerasManager::GetActiveCameraProjectionMatrix());
				mesh->DrawSubMesh(i);

				glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				glStencilMask(0x00);
				glDisable(GL_DEPTH_TEST);

				Transform outlineTransform = *owner->GetTransform();
				outlineTransform.SetLocalScale(1.05f * outlineTransform.GetLocalScale());
				editorOutlineMaterial->Draw(outlineTransform.GetTransformMatrix(), CamerasManager::GetActiveCameraViewMatrix(), CamerasManager::GetActiveCameraProjectionMatrix());
				mesh->DrawSubMesh(i);

				glStencilMask(0xFF);
				glStencilFunc(GL_ALWAYS, 0, 0xFF);
				glEnable(GL_DEPTH_TEST);
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