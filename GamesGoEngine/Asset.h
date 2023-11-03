#pragma once

#include <string>

#include "ReflectableObject.h"

namespace GamesGoEngine
{
	/*
	 * Asset types recognized by AssetsManager, each asset is categorized based on file extension
	 * Asset that require extra functionality will have dedicated classes
	 * Asset that only need to have their type recognized won't have dedicated classes
	 */
	enum class AssetType
	{
		Material,
		Mesh,
		Project,
		Shader,
		Texture,
		Unsupported
	};

	class Asset : public ReflectableObject
	{
	public:
		Asset();

		virtual void Load(AssetType type, std::string name, std::string path);
		virtual void Unload();
		void Save();

	private:
		void ReadFromPropertiesFile();

	public:
		AssetType GetType() const;
		std::string GetName() const;
		std::string GetPath() const;
		std::string GetPropertiesFilePath() const;
		// Changes asset file name along with its corresponding properties file
		void SetName(std::string newName);
		
	protected:
		virtual void InitMetaData() override;

		std::string path;
		std::string propertiesPath;

	private:
		static std::string FieldValueToString(const Field& field);

		AssetType type;
		std::string name;
		std::string fileExtension;
		std::string fileDirectory;

	public:
		static constexpr int NAME_MAX_LENGTH = 256;
	};
}