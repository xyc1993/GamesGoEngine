#include "ReflectionFactory.h"

#include "AmbientLight.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "EditorMovement.h"
#include "MeshRenderer.h"
#include "PostProcessRenderer.h"
#include "PositionOscillator.h"
#include "Rotator.h"
#include "ScaleOscillator.h"

namespace GamesGoEngine
{
	ReflectableObjectsFactory ReflectionFactory::reflectableObjectsFactory;
	bool ReflectionFactory::componentsInitialized = false;

	ReflectableObjectsFactory ReflectionFactory::GetReflectableObjectsFactory()
	{
		if (!componentsInitialized)
		{
			// TODO: think of more elegant way of registering components
			REGISTER_CLASS(AmbientLight);
			REGISTER_CLASS(Camera);
			REGISTER_CLASS(DirectionalLight);
			REGISTER_CLASS(EditorMovement);
			REGISTER_CLASS(MeshRenderer);
			REGISTER_CLASS(PointLight);
			REGISTER_CLASS(PositionOscillator);
			REGISTER_CLASS(PostProcessRenderer);
			REGISTER_CLASS(Rotator);
			REGISTER_CLASS(ScaleOscillator);
			REGISTER_CLASS(SkyboxRenderer);
			REGISTER_CLASS(SpotLight);
			componentsInitialized = true;
		}

		return reflectableObjectsFactory;
	}
}