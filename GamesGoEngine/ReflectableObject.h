#pragma once

#include <string>
#include <vector>

namespace GamesGoEngine
{
	#define TO_STRING(X) (#X)
	#define CLASS_NAME(CLASS) (TO_STRING(CLASS))

	struct Field
	{
		// Type of the field parsed to string, used to identify what kind of field we're dealing with
		std::string typeName;
		// Name of the field, used for labels
		std::string fieldName;
		// Actual pointer to the relevant data
		void* fieldPointer;
		// TODO: option to provide a setter function
		// TODO: think about pointer fields (think what type of pointer fields you can have, maybe rely on polymorphism if common parent? or templates? some extra identifier?)
		// TODO: think about container fields
	};

	struct ClassMetaData
	{
		std::string className;
		std::vector<Field> classFields;
	};

	#define FILL_FIELD_META_DATA(FIELD_DATA, TYPE_NAME, FIELD_NAME, FIELD) \
		((FIELD_DATA).typeName) = TO_STRING(TYPE_NAME);\
		((FIELD_DATA).fieldName) = TO_STRING(FIELD_NAME);\
		((FIELD_DATA).fieldPointer) = &(FIELD);

	#define ADD_FIELD_META_DATA(META_DATA_STRUCT, TYPE_NAME, FIELD_NAME, FIELD) {\
		Field FIELD_STRUCT; \
		FILL_FIELD_META_DATA(FIELD_STRUCT, TYPE_NAME, FIELD_NAME, FIELD) \
		(META_DATA_STRUCT).classFields.push_back(FIELD_STRUCT); \
		}

	/*
	 * Class holding all of its meta data
	 */
	class ReflectableObject
	{
	public:
		ClassMetaData GetMetaData() const;
		virtual void InitMetaData();

	protected:
		ClassMetaData metaData;
	};
}