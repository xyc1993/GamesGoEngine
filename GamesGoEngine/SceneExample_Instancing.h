#pragma once

#include <memory>
#include <GL/glew.h>

#include "SceneExample.h"

class MeshImported;

class SceneExample_Instancing : public SceneExample
{
public:
	SceneExample_Instancing();

private:
	void InitAsteroidsUninstanced(int asteroidsNumber, std::shared_ptr<MeshImported> asteroidMesh, GLuint asteroidTexture, glm::mat4* modelMatrices);
	void InitAsteroidsInstanced(int asteroidsNumber, std::shared_ptr<MeshImported> asteroidMesh, GLuint asteroidTexture, glm::mat4* modelMatrices);
};