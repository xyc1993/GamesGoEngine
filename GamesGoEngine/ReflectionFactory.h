#pragma once

#include <map>
#include <string>
#include <vcruntime_typeinfo.h>
#include <vector>

namespace GamesGoEngine
{
	template <class T> void* Constructor() { return (void*)new T(); }

	struct ReflectableObjectsFactory
	{
	public:
		typedef void* (*ConstructorT)();
		typedef std::map<std::string, ConstructorT> Constructors;
		Constructors classes;

		template <class T>
		void RegisterClass()
		{
			classes.insert(std::make_pair(GetParsedClassName<T>(), &Constructor<T>));
		}

		void* Construct(std::string const& className)
		{
			Constructors::iterator i = classes.find(className);
			if (i == classes.end()) return nullptr;
			return i->second();
		}

	private:
		// Parses T class name to more GUI friendly string (just the class name)
		template <class T>
		std::string GetParsedClassName()
		{
			std::string parsedString = typeid(T).name();

			// Remove class keyword
			const std::string classKeyword = "class";
			std::string::size_type i = parsedString.find(classKeyword);
			if (i != std::string::npos)
			{
				parsedString.erase(i, classKeyword.length() + 1); // '+1' to remove whitespace between 'class' and class name
			}	

			// If there's namespace included in the class name, remove it (C++ format would be class namespace::className)
			i = parsedString.find("::");
			if (i != std::string::npos)
			{
				parsedString.erase(0, i + 2); // '+2' because we want to remove everything before "::" and "::" as well
			}	

			return parsedString;
		}
	};
	
	// Used for a generic creation of components, for now it's assumed that only components are registered here, might change in the future
	class ReflectionFactory
	{
	public:
		static ReflectableObjectsFactory* GetReflectableObjectsFactory();

	private:
		static ReflectableObjectsFactory* reflectableObjectsFactory;
	};

	#define REGISTER_CLASS(CLASS) ReflectionFactory::GetReflectableObjectsFactory()->RegisterClass<CLASS>()

	template <typename T>
	struct ClassRegistrationStruct
	{
		ClassRegistrationStruct()
		{
			REGISTER_CLASS(T);
		}
	};

	#define REGISTER_CLASS_STATIC(CLASS) static ClassRegistrationStruct<CLASS> CLASS##RegistrationStruct
}