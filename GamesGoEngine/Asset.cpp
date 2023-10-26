#include "Asset.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "AssetsManager.h"

namespace GamesGoEngine
{
	Asset::Asset()
	{
		type = AssetType::Unsupported;
		name = "UNKNOWN NAME";
	}

	void Asset::Load(AssetType type, std::string name, std::string path)
	{
		InitMetaData();
		
		this->type = type;
		this->name = name;
		this->path = path;

		const std::filesystem::path filePath = path;
		this->fileExtension = filePath.extension().string();
		this->fileDirectory = filePath.parent_path().string();

		this->propertiesPath = path;
		this->propertiesPath.append(AssetsManager::GetAssetPropertiesFileExtension());

		// Read from file AFTER initializing meta data and properties path
		ReadFromPropertiesFile();
	}

	void Asset::Unload()
	{

	}

	void Asset::Save()
	{
		const std::vector<Field> assetFields = GetMetaData().classFields;
		if (assetFields.empty())
		{
			return;
		}

		std::ofstream outfile(propertiesPath);
		for (size_t i = 0; i < assetFields.size(); i++)
		{
			std::string line;

			line.append(assetFields[i].typeName);
			line.append(" ");
			line.append(assetFields[i].fieldName);
			line.append(" ");
			line.append(FieldValueToString(assetFields[i]));

			outfile << line << "\n";
		}
		outfile.close();
		
	}

	void Asset::ReadFromPropertiesFile()
	{
		std::ifstream propertiesFile(propertiesPath);
		if (propertiesFile.good())
		{
			std::string line;
			while (std::getline(propertiesFile, line))
			{
				std::istringstream iss(line);
				std::string typeName, fieldName, fieldValue;
				iss >> typeName >> fieldName >> fieldValue;

				const std::vector<Field> assetFields = GetMetaData().classFields;
				for (size_t i = 0; i < assetFields.size(); i++)
				{
					if (assetFields[i].fieldName == fieldName)
					{
						// TODO: add more types
						if (typeName == "bool")
						{
							bool* boolField = static_cast<bool*>(assetFields[i].fieldPointer);
							*boolField = static_cast<bool>(std::stoi(fieldValue));
						}

						if (typeName == "std::string")
						{
							std::string* stringField = static_cast<std::string*>(assetFields[i].fieldPointer);
							*stringField = fieldValue;
						}
					}
				}
			}
		}
		propertiesFile.close();
	}

	AssetType Asset::GetType() const
	{
		return type;
	}

	std::string Asset::GetName() const
	{
		return name;
	}

	std::string Asset::GetPath() const
	{
		return path;
	}

	std::string Asset::GetPropertiesFilePath() const
	{
		return propertiesPath;
	}

	void Asset::SetName(std::string newName)
	{
		newName.append(fileExtension);
		if (name != newName)
		{
			// Rename file
			std::string oldPath = path;
			std::string newPath = fileDirectory;
			newPath.append("/");
			newPath.append(newName);
			rename(oldPath.c_str(), newPath.c_str());

			// Rename properties file
			std::string newPropertiesPath = newPath;
			newPropertiesPath.append(AssetsManager::GetAssetPropertiesFileExtension());
			rename(propertiesPath.c_str(), newPropertiesPath.c_str());

			// Set new values
			name = newName;
			path = newPath;
			propertiesPath = newPropertiesPath;			
		}
	}

	void Asset::InitMetaData()
	{
		metaData.className = CLASS_NAME(Asset);
	}

	std::string Asset::FieldValueToString(const Field& field)
	{
		std::string value;
		std::stringstream ss;

		// TODO: add more types
		if (field.typeName == "bool")
		{
			bool* boolField = static_cast<bool*>(field.fieldPointer);
			ss << *boolField;
			value.append(ss.str());
		}

		if (field.typeName == "std::string")
		{
			value = *static_cast<std::string*>(field.fieldPointer);
		}

		return value;
	}
}