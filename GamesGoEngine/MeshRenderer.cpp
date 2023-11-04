#include "MeshRenderer.h"

#include "AssetMaterial.h"
#include "AssetsManager.h"
#include "AssetMesh.h"
#include "CamerasManager.h"
#include "GameObject.h"
#include "MeshImported.h"

namespace GamesGoEngine
{
	REGISTER_CLASS_STATIC(MeshRenderer);

	MeshRenderer::MeshRenderer()
	{
		assetMesh = nullptr;
		mesh = nullptr;
		assetMaterials.clear();
		materialList.clear();
		SetRenderQueuePosition(RenderQueuePosition::Opaque);
		RenderingManager::AddMeshRenderer(this);
	}

	MeshRenderer::~MeshRenderer()
	{
		RenderingManager::RemoveMeshRenderer(this);
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

	void MeshRenderer::SetMaterial(const std::shared_ptr<Material>& material, size_t materialIndex, bool resetAssetRef)
	{
		if (materialIndex < materialList.size())
		{
			if (materialList[materialIndex] != material)
			{
				materialList[materialIndex].reset();
				materialList[materialIndex] = material;

				// Resets asset material reference when needed, for example when using materials that are not from assets but procedurally generated
				if (resetAssetRef)
				{
					assetMaterials[materialIndex] = nullptr;
				}
			}
		}
	}

	void MeshRenderer::SetMaterial(const std::string& path)
	{
		SetMaterial(path, 0);
	}

	void MeshRenderer::SetMaterial(const std::string& path, size_t materialIndex)
	{
		Asset* asset = AssetsManager::GetAsset(path);
		if (AssetMaterial* assetMaterial = dynamic_cast<AssetMaterial*>(asset))
		{
			if (materialIndex < assetMaterials.size())
			{
				assetMaterials[materialIndex] = assetMaterial;
			}
			SetMaterial(assetMaterial->GetMaterial(), materialIndex, false);
		}
	}

	std::string MeshRenderer::GetMaterialName() const
	{
		return GetMaterialName(0);
	}

	std::string MeshRenderer::GetMaterialName(size_t materialIndex) const
	{
		if (materialIndex < assetMaterials.size() && materialIndex < materialList.size())
		{
			if (assetMaterials[materialIndex] != nullptr)
			{
				return assetMaterials[materialIndex]->GetName();
			}
			else if (materialList[materialIndex] != nullptr)
			{
				return "UNNAMED MATERIAL";
			}
		}
		return "NO MATERIAL";
	}

	void MeshRenderer::SetMesh(const std::shared_ptr<MeshBase>& mesh, bool resetAssetRef)
	{
		if (this->mesh != mesh)
		{
			// firstly set mesh
			this->mesh.reset();
			this->mesh = mesh;

			// finally, do material cleanup & set proper size to the list
			CleanMaterialList();
			materialList.resize(this->mesh->GetSubMeshesCount());
			assetMaterials.resize(this->mesh->GetSubMeshesCount());

			// Resets asset mesh reference when needed, for example when using meshes that are not from assets but procedurally generated
			if (resetAssetRef)
			{
				this->assetMesh = nullptr;
			}
		}
	}

	void MeshRenderer::SetMesh(const std::string& path)
	{
		Asset* asset = AssetsManager::GetAsset(path);
		if (AssetMesh* assetMesh = dynamic_cast<AssetMesh*>(asset))
		{
			this->assetMesh = assetMesh;
			SetMesh(this->assetMesh->GetMesh(), false);
		}
	}

	std::string MeshRenderer::GetMeshName() const
	{
		if (mesh == nullptr)
		{
			return "NO MESH";
		}
		return assetMesh != nullptr ? assetMesh->GetName() : "UNNAMED MESH";
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

	void MeshRenderer::SetIsCastingShadow(bool shadowCastingEnabled)
	{
		isCastingShadow = shadowCastingEnabled;
	}

	bool MeshRenderer::IsCastingShadow() const
	{
		return isCastingShadow;
	}

	// TODO: In the future the check should be per material, not renderer!
	// for simplicity let's assume that every material is of the same type, it's not obvious, but it's ok for current implementation of forward/deferred shading use
	LightModelType MeshRenderer::GetLightModelType() const
	{
		if (!materialList.empty())
		{
			if (materialList[0] != nullptr)
			{
				return materialList[0]->GetLightModel();
			}
		}
		return LightModelType::Unlit;
	}

	void MeshRenderer::CleanMaterialList()
	{
		Renderer::CleanMaterialList();

		assetMaterials.clear();
	}

	void MeshRenderer::InitMetaData()
	{
		metaData.className = CLASS_NAME(MeshRenderer);

		ADD_FIELD_META_DATA(metaData, bool, isCastingShadow, this->isCastingShadow)
	}
}