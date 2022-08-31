#include "SceneExample_Instancing.h"

#include "MeshImported.h"
#include "MeshRenderer.h"
#include "TextureLoader.h"

SceneExample_Instancing::SceneExample_Instancing()
{
	/*
	GameObject* planetObject = new GameObject();

	MeshRenderer* planetMeshRenderer = new MeshRenderer();
	std::shared_ptr<MeshImported> planetMesh = std::make_shared<MeshImported>((GLchar*)"res/planet/planet.obj");
	planetMeshRenderer->SetMesh(planetMesh);

	std::shared_ptr<Material> planetMaterial = std::make_shared<Material>("res/shaders/unlitTextured.vert.glsl", "res/shaders/unlitTextured.frag.glsl");
	planetMaterial->SetTextureByPath("mainTexture", 0, (GLchar*)"res/planet/mars.png");
	planetMeshRenderer->SetMaterial(planetMaterial, 0);

	planetObject->AddComponent(planetMeshRenderer);

	std::string planetName = "planet";
	planetObject->SetName(planetName);

	scene->AddGameObject(planetObject);
	*/

	GameObject* asteroidObject = new GameObject();

	MeshRenderer* asteroidMeshRenderer = new MeshRenderer();
	std::shared_ptr<MeshImported> asteroidMesh = std::make_shared<MeshImported>((GLchar*)"res/rock/rock.obj");
	asteroidMeshRenderer->SetMesh(asteroidMesh);

	std::shared_ptr<Material> asteroidMaterial = std::make_shared<Material>("res/shaders/unlitTextured.vert.glsl", "res/shaders/unlitTextured.frag.glsl");
	asteroidMaterial->SetTextureByPath("mainTexture", 0, (GLchar*)"res/rock/rock.png");
	asteroidMeshRenderer->SetMaterial(asteroidMaterial, 0);

	asteroidObject->AddComponent(asteroidMeshRenderer);

	std::string asteroidName = "asteroid";
	asteroidObject->SetName(asteroidName);

	scene->AddGameObject(asteroidObject);

	GameObject* editorSpectatorObject = AddEditorSpectator();
	editorSpectatorObject->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));
}