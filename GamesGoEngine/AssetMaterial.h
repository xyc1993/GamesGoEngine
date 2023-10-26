#pragma once

#include <memory>

#include "Asset.h"

namespace GamesGoEngine
{
	class Material;

	class AssetMaterial : public Asset
	{
	public:
		virtual void Load(AssetType type, std::string name, std::string path) override;
		virtual void Unload() override;

		std::string GetVertexShaderPath() const;
		std::string GetFragmentShaderPath() const;
		void SetVertexShaderPath(std::string path);
		void SetFragmentShaderPath(std::string path);
		std::shared_ptr<Material> GetMaterial() const;

	private:
		void TrySetShader();

	protected:
		virtual void InitMetaData() override;

	private:
		std::shared_ptr<Material> material;
		std::string vertexShaderPath;
		std::string fragmentShaderPath;
	};
}