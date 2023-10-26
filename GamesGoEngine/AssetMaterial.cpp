#include "AssetMaterial.h"

#include <memory>

#include "Material.h"

namespace GamesGoEngine
{
	void AssetMaterial::Load(AssetType type, std::string name, std::string path)
	{
		Asset::Load(type, name, path);

		material = std::make_shared<Material>();
		TrySetShader();
	}

	void AssetMaterial::Unload()
	{
		Asset::Unload();
	}

	std::string AssetMaterial::GetVertexShaderPath() const
	{
		return vertexShaderPath;
	}

	std::string AssetMaterial::GetFragmentShaderPath() const
	{
		return fragmentShaderPath;
	}

	void AssetMaterial::SetVertexShaderPath(std::string path)
	{
		vertexShaderPath = path;
		TrySetShader();
	}

	void AssetMaterial::SetFragmentShaderPath(std::string path)
	{
		fragmentShaderPath = path;
		TrySetShader();
	}

	std::shared_ptr<Material> AssetMaterial::GetMaterial() const
	{
		return material;
	}

	void AssetMaterial::TrySetShader()
	{
		if (!vertexShaderPath.empty() && !fragmentShaderPath.empty())
		{
			//material->SetShader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
		}
	}

	void AssetMaterial::InitMetaData()
	{
		metaData.className = CLASS_NAME(AssetMaterial);

		ADD_FIELD_META_DATA(metaData, std::string, vertexShaderPath, this->vertexShaderPath);
		ADD_FIELD_META_DATA(metaData, std::string, fragmentShaderPath, this->fragmentShaderPath);
	}
}