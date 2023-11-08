#pragma once

#include <memory>

#include "SceneExample.h"

namespace GamesGoEngine
{
	class MeshImported;

	class SceneExample_Instancing : public SceneExample
	{
	public:
		SceneExample_Instancing();

	private:
		void InitAsteroidsUninstanced(int asteroidsNumber, std::shared_ptr<MeshImported> asteroidMesh, unsigned int asteroidTexture, glm::mat4* modelMatrices);
		void InitAsteroidsInstanced(int asteroidsNumber, std::shared_ptr<MeshImported> asteroidMesh, unsigned int asteroidTexture, glm::mat4* modelMatrices);
	};
}