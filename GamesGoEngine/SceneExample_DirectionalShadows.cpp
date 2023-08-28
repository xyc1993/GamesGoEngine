#include "SceneExample_DirectionalShadows.h"

#include "MeshImported.h"
#include "MeshPrimitiveCube.h"
#include "MeshPrimitiveQuad.h"
#include "MeshPrimitiveSphere.h"
#include "MeshRenderer.h"

namespace GamesGoEngine
{
	SceneExample_DirectionalShadows::SceneExample_DirectionalShadows()
	{
		std::shared_ptr<MeshPrimitiveCube> cubeMesh = std::make_shared<MeshPrimitiveCube>();
		std::shared_ptr<MeshPrimitiveSphere> sphereMesh = std::make_shared<MeshPrimitiveSphere>();
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

		// Add cubes & sphere
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
			std::string name;
			if (i == 0)
			{
				name = "sphere_";
				cubeMeshRenderer->SetMesh(sphereMesh);
			}
			else
			{
				name = "cube_";
				cubeMeshRenderer->SetMesh(cubeMesh);
			}
			cubeMeshRenderer->SetMaterial(woodMaterial);
			cubeObject->AddComponent(cubeMeshRenderer);
			name.append(std::to_string(i));
			cubeObject->SetName(name);
			cubeObject->GetTransform()->SetPosition(cubePositions[i]);
			cubeObject->GetTransform()->SetRotationEulerDegrees(cubeRotations[i]);
			scene->AddGameObject(cubeObject);
		}

		// Add complex mesh
		GameObject* nanoSuitObject = new GameObject();
		nanoSuitObject->GetTransform()->SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));
		nanoSuitObject->GetTransform()->SetRotationEulerDegrees(glm::vec3(0.0f, 135.0f, 0.0f));
		nanoSuitObject->GetTransform()->SetScale(glm::vec3(0.2f));
		MeshRenderer* nanoSuitMeshRenderer = new MeshRenderer();
		std::shared_ptr<MeshImported> nanoSuitMesh = std::make_shared<MeshImported>((GLchar*)"res/nanosuit/nanosuit.obj");
		nanoSuitMeshRenderer->SetMesh(nanoSuitMesh);

		const int NANOSUIT_MATERIALS_NUMBER = 7;
		std::shared_ptr<Material> nanoSuitMaterials[NANOSUIT_MATERIALS_NUMBER];

		std::vector<GLchar*> nanoSuitTexturePaths;
		nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/glass_dif.png");
		nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/leg_dif.png");
		nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/hand_dif.png");
		nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/glass_dif.png"); // this one is small 'lights' on the helmet under visor, no specific texture was given for those
		nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/arm_dif.png");
		nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/helmet_diff.png");
		nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/body_dif.png");

		for (int i = 0; i < NANOSUIT_MATERIALS_NUMBER; i++)
		{
			nanoSuitMaterials[i] = std::make_shared<Material>("res/shaders/litShadowSimple.vert.glsl", "res/shaders/litShadowSimple.frag.glsl");
			nanoSuitMaterials[i]->SetTextureByPath((GLchar*)"diffuseTexture", 0, nanoSuitTexturePaths[i]);
			nanoSuitMaterials[i]->SetLightModel(LightModelType::LitForward);
			nanoSuitMeshRenderer->SetMaterial(nanoSuitMaterials[i], i);
		}

		nanoSuitObject->AddComponent(nanoSuitMeshRenderer);

		std::string nanoSuitName = "nanosuit";
		nanoSuitObject->SetName(nanoSuitName);

		scene->AddGameObject(nanoSuitObject);

		// Add source of light
		GameObject* directionalLightObject = new GameObject();
		directionalLightObject->GetTransform()->SetRotationEulerDegrees(glm::vec3(30.0f, -45.0f, 0.0f));
		DirectionalLight* directionalLight = new DirectionalLight(glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
		directionalLightObject->AddComponent(directionalLight);
		directionalLightObject->SetName("directional_light");
		scene->AddGameObject(directionalLightObject);

		AddEditorSpectator();
	}
}