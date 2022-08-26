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
	if (owner != nullptr)
	{
		if (owner->IsSelected())
		{
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
		}
		else
		{
			glStencilMask(0x00);
		}

		Renderer::Draw();

		glStencilMask(0x00);
	}
}

void MeshRenderer::Draw(Material* material)
{
	Renderer::Draw(material);
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