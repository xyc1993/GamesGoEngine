#include "MeshRenderer.h"

#include "CamerasManager.h"
#include "GameObject.h"
#include "MeshImported.h"

MeshRenderer::MeshRenderer()
{
	mesh = nullptr;
	materialList.clear();
	SetRenderQueuePosition(RenderQueuePosition::Opaque);
	RenderingManager::AddMeshRenderer(this);
}

void MeshRenderer::OnSelected()
{
	SetRenderQueuePosition(RenderQueuePosition::EditorOutline, true);
}

void MeshRenderer::OnDeselected()
{
	SetRenderQueuePosition(deselectedRenderQueuePosition, true);
}

void MeshRenderer::Draw()
{
	if (owner != nullptr && mesh != nullptr)
	{
		if (!owner->IsSelected())
		{
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
		else
		{
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
					// for non imported meshes we'll scale the mesh to have an outline, imported meshes use special vertex shader to achieve similar effect
					if (!mesh->IsImportedMesh())
					{
						outlineTransform.SetLocalScale(1.05f * outlineTransform.GetLocalScale());
					}
					RenderingManager::GetEditorOutlineMaterial(mesh->IsImportedMesh())->Draw(outlineTransform.GetTransformMatrix(), CamerasManager::GetActiveCameraViewMatrix(), CamerasManager::GetActiveCameraProjectionMatrix());
					mesh->DrawSubMesh(i);

					glStencilMask(0xFF);
					glStencilFunc(GL_ALWAYS, 0, 0xFF);
					glEnable(GL_DEPTH_TEST);
				}
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

void MeshRenderer::SetRenderQueuePosition(RenderQueuePosition position, bool selected)
{
	SetRenderQueuePosition(static_cast<int>(position), selected);
}

void MeshRenderer::SetRenderQueuePosition(int position, bool selected)
{
	currentRenderQueuePosition = position;
	if (!selected)
	{
		deselectedRenderQueuePosition = position;
	}
	RenderingManager::SortMeshRenderers();
}

int MeshRenderer::GetRenderQueuePosition() const
{
	return currentRenderQueuePosition;
}