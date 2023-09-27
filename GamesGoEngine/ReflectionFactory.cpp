#include "ReflectionFactory.h"

namespace GamesGoEngine
{
	ReflectableObjectsFactory* ReflectionFactory::reflectableObjectsFactory = nullptr;

	ReflectableObjectsFactory* ReflectionFactory::GetReflectableObjectsFactory()
	{
		if (reflectableObjectsFactory == nullptr)
		{
			reflectableObjectsFactory = new ReflectableObjectsFactory();
		}
		return reflectableObjectsFactory;
	}
}