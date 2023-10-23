#pragma once

#include <string>

#include "ReflectableObject.h"

namespace GamesGoEngine
{
	enum class AssetType
	{
		Project,
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
		
	protected:
		virtual void InitMetaData() override;

		std::string path;
		std::string propertiesPath;

	private:
		static std::string FieldValueToString(const Field& field);

		AssetType type;
		std::string name;
	};
}