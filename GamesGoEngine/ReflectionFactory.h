#pragma once

#include <map>
#include <string>
#include <vector>

namespace GamesGoEngine
{
	template <class T> void* Constructor() { return (void*)new T(); }

	struct ReflectableObjectsFactory
	{
		typedef void* (*ConstructorT)();
		typedef std::map<std::string, ConstructorT> Constructors;
		Constructors classes;

		template <class T>
		void RegisterClass(std::string const& className)
		{
			classes.insert(std::make_pair(className, &Constructor<T>));
		}

		void* Construct(std::string const& className)
		{
			Constructors::iterator i = classes.find(className);
			if (i == classes.end()) return nullptr;
			return i->second();
		}
	};
	
	// Used for a generic creation of components, for now it's assumed that only components are registered here, might change in the future
	class ReflectionFactory
	{
	public:
		static ReflectableObjectsFactory GetReflectableObjectsFactory();

	private:
		static ReflectableObjectsFactory reflectableObjectsFactory;
		static bool componentsInitialized;
	};

	#define REGISTER_CLASS(CLASS) ReflectionFactory::reflectableObjectsFactory.RegisterClass<CLASS>(#CLASS)
}