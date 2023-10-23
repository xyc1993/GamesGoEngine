#include "Asset.h"

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
				if (!(iss >> typeName >> fieldName >> fieldValue))
				{
					throw std::runtime_error("Couldn't read line from asset properties file, line is parsed incorrectly!");
				}

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

		return value;
	}
}