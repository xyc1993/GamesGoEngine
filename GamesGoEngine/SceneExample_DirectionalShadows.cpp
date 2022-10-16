#include "SceneExample_DirectionalShadows.h"

#include "MeshPrimitiveCube.h"
#include "MeshPrimitiveQuad.h"
#include "MeshRenderer.h"

SceneExample_DirectionalShadows::SceneExample_DirectionalShadows()
{
	std::shared_ptr<MeshPrimitiveCube> cubeMesh = std::make_shared<MeshPrimitiveCube>();
	std::shared_ptr<MeshPrimitiveQuad> quadMesh = std::make_shared<MeshPrimitiveQuad>();

	std::shared_ptr<Material> woodMaterial = std::make_shared<Material>("res/shaders/litShadowSimple.vert.glsl", "res/shaders/litShadowSimple.frag.glsl");
	woodMaterial->SetTextureByPath((GLchar*)"diffuseTexture", 0, (GLchar*)"res/textures/wood.png");
	woodMaterial->SetLightModel(LightModelType::LitForward);

	// Add floor
	GameObject* floorObject = new GameObject();
	MeshRenderer* floorMeshRenderer = new MeshRenderer();
	floorMeshRenderer->SetMesh(quadMesh);
	floorMeshRenderer->SetMaterial(woodMaterial);
	floorObject->AddComponent(floorMeshRenderer);
	floorObject->SetName("floor");
	floorObject->GetTransform()->SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));
	floorObject->GetTransform()->SetRotationEulerDegrees(glm::vec3(-90.0f, 0.0f, 0.0f));
	floorObject->GetTransform()->SetScale(glm::vec3(20.0f));
	scene->AddGameObject(floorObject);

	// Add cubes
	std::vector<glm::vec3> cubePositions;
	cubePositions.push_back(glm::vec3(2.0f, -1.5f, -3.5f));
	cubePositions.push_back(glm::vec3(-3.0f, 1.0f, -2.0f));
	cubePositions.push_back(glm::vec3(0.0f, -1.0f, -5.5f));

	std::vector<glm::vec3> cubeRotations;
	cubeRotations.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	cubeRotations.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	cubeRotations.push_back(glm::vec3(45.0f, 45.0f, 0.0f));

	for (size_t i = 0; i < cubePositions.size(); i++)
	{
		GameObject* cubeObject = new GameObject();
		MeshRenderer* cubeMeshRenderer = new MeshRenderer();
		cubeMeshRenderer->SetMesh(cubeMesh);
		cubeMeshRenderer->SetMaterial(woodMaterial);
		cubeObject->AddComponent(cubeMeshRenderer);
		std::string name = "cube_";
		name.append(std::to_string(i));
		cubeObject->SetName(name);
		cubeObject->GetTransform()->SetPosition(cubePositions[i]);
		cubeObject->GetTransform()->SetRotationEulerDegrees(cubeRotations[i]);
		scene->AddGameObject(cubeObject);
	}

	// Add source of light
	GameObject* directionalLightObject = new GameObject();
	directionalLightObject->GetTransform()->SetRotationEulerDegrees(glm::vec3(30.0f, -45.0f, 0.0f));
	DirectionalLight* directionalLight = new DirectionalLight(glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
	directionalLightObject->AddComponent(directionalLight);
	directionalLightObject->SetName("directional_light");
	scene->AddGameObject(directionalLightObject);

	AddEditorSpectator();
}