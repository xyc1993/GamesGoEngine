#include "SceneExample_LitForward.h"

#include "Camera.h"
#include "DirectionalLight.h"
#include "EditorMovement.h"
#include "Material.h"
#include "MeshImported.h"
#include "MeshPrimitiveCube.h"
#include "MeshPrimitiveSphere.h"
#include "MeshRenderer.h"
#include "PointLight.h"
#include "Skybox.h"
#include "SpotLight.h"

extern GLfloat currentTime;

SceneExample_LitForward::SceneExample_LitForward()
{
	glm::vec3 litBoxesPositions[] = {
		glm::vec3(3.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	glm::vec3 litBoxesRotations[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(30.0f, 15.0f, 0.0f),
		glm::vec3(60.0f, 30.0f, 0.0f),
		glm::vec3(90.0f, 45.0f, 0.0f),
		glm::vec3(120.0f,60.0f, 0.0f),
		glm::vec3(150.0f,75.0f, 0.0f),
		glm::vec3(180.0f,90.0f, 0.0f),
		glm::vec3(210.0f,105.0f, 0.0f),
		glm::vec3(240.0f,120.0f, 0.0f),
		glm::vec3(270.0f,135.0f, 0.0f)
	};

	std::vector<const GLchar*> skyboxTextures;
	skyboxTextures.push_back("res/images/skybox/right.tga");
	skyboxTextures.push_back("res/images/skybox/left.tga");
	skyboxTextures.push_back("res/images/skybox/top.tga");
	skyboxTextures.push_back("res/images/skybox/bottom.tga");
	skyboxTextures.push_back("res/images/skybox/back.tga");
	skyboxTextures.push_back("res/images/skybox/front.tga");

	Material* skyboxMaterial = new Material("res/shaders/skybox.vert.glsl", "res/shaders/skybox.frag.glsl");
	skyboxMaterial->SetCubeTexture("skybox", 1, skyboxTextures);

	Skybox* skyboxComponent = new Skybox();
	skyboxComponent->SetMaterial(skyboxMaterial);

	GameObject* skyboxObject = new GameObject();
	skyboxObject->AddComponent(skyboxComponent);

	std::string skyboxName = "skybox";
	skyboxObject->SetName(skyboxName);

	scene->AddGameObject(skyboxObject);
	
	MeshPrimitiveCube* cubeMesh = new MeshPrimitiveCube();
	//MeshPrimitiveQuad* quadMesh = new MeshPrimitiveQuad();
	MeshPrimitiveSphere* sphereMesh = new MeshPrimitiveSphere();

	Material* lampMaterial = new Material("res/shaders/lamp.vert.glsl", "res/shaders/lamp.frag.glsl");
	
	for (int i = 0; i < LAMPS_NUMBER; i++)
	{
		GameObject* lampObject = new GameObject();
		MeshRenderer* lampMeshRenderer = new MeshRenderer();
		lampMeshRenderer->SetMesh(sphereMesh);
		lampMeshRenderer->SetMaterial(lampMaterial);
		lampObject->AddComponent(lampMeshRenderer);
		lampObject->GetTransform()->SetPosition(startPointLightPositions[i]);
		lampObject->GetTransform()->SetScale(glm::vec3(0.2f));

		PointLight* pointLight = new PointLight(glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f);
		lampObject->AddComponent(pointLight);

		std::string name = "sphere_lamp_";
		name.append(std::to_string(i));
		lampObject->SetName(name);

		lampObjectsIndices[i] = scene->AddGameObject(lampObject);
	}

	Material* cubeLitMaterial = new Material("res/shaders/lighting.vert.glsl", "res/shaders/lighting.frag.glsl");
	cubeLitMaterial->SetTexture((GLchar*)"material.diffuse", 0, (GLchar*)"res/box/container2_diffuse.png");
	cubeLitMaterial->SetTexture((GLchar*)"material.specular", 1, (GLchar*)"res/box/container2_specular.png");
	cubeLitMaterial->SetFloat((GLchar*)"material.shininess", 32.0f);
	cubeLitMaterial->SetLightModel(LightModelType::LitForward);

	const int LIT_BOXES_NUMBER = 10;
	for (int i = 0; i < LIT_BOXES_NUMBER; i++)
	{
		GameObject* litBoxesObjects = new GameObject();
		MeshRenderer* litBoxesMeshRenderer = new MeshRenderer();
		litBoxesMeshRenderer->SetMesh(cubeMesh);
		litBoxesMeshRenderer->SetMaterial(cubeLitMaterial);
		litBoxesObjects->AddComponent(litBoxesMeshRenderer);
		litBoxesObjects->GetTransform()->SetPosition(litBoxesPositions[i]);
		litBoxesObjects->GetTransform()->SetRotationEulerDegrees(litBoxesRotations[i]);

		std::string name = "lit_box_";
		name.append(std::to_string(i));
		litBoxesObjects->SetName(name);

		scene->AddGameObject(litBoxesObjects);
	}

	GameObject* nanoSuitObject = new GameObject();
	nanoSuitObject->GetTransform()->SetPosition(glm::vec3(0.0f, -1.75f, 0.0f));
	nanoSuitObject->GetTransform()->SetScale(glm::vec3(0.2f));
	MeshRenderer* nanoSuitMeshRenderer = new MeshRenderer();
	MeshImported* nanoSuitMesh = new MeshImported((GLchar*)"res/nanosuit/nanosuit.obj");
	nanoSuitMeshRenderer->SetMesh(nanoSuitMesh);

	const int NANOSUIT_MATERIALS_NUMBER = 7;
	Material* nanoSuitMaterials[] = {
		new Material(), new Material(), new Material(),
		new Material(), new Material(), new Material(),
		new Material()
	};

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
		nanoSuitMaterials[i] = new Material("res/shaders/lighting.vert.glsl", "res/shaders/lighting.frag.glsl");
		nanoSuitMaterials[i]->SetTexture((GLchar*)"material.diffuse", 0, nanoSuitTexturePaths[i]);
		nanoSuitMaterials[i]->SetFloat((GLchar*)"material.shininess", 32.0f);
		nanoSuitMaterials[i]->SetLightModel(LightModelType::LitForward);
		nanoSuitMeshRenderer->SetMaterial(nanoSuitMaterials[i], i);
	}

	nanoSuitObject->AddComponent(nanoSuitMeshRenderer);

	std::string nanoSuitName = "nanosuit";
	nanoSuitObject->SetName(nanoSuitName);

	scene->AddGameObject(nanoSuitObject);

	GameObject* editorSpectatorObject = new GameObject();
	editorSpectatorObject->GetTransform()->SetRotationEulerDegrees(glm::vec3(0.0f, 180.0f, 0.0f));
	editorSpectatorObject->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0, 3.0f));

	EditorMovement* editorMovementComponent = new EditorMovement();
	Camera* cameraComponent = new Camera();
	SpotLight* spotLight = new SpotLight(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f),
		1.0f, 0.09f, 0.032f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)));

	editorSpectatorObject->AddComponent(editorMovementComponent);
	editorSpectatorObject->AddComponent(cameraComponent);
	editorSpectatorObject->AddComponent(spotLight);

	std::string editorSpectatorName = "editor_spectator";
	editorSpectatorObject->SetName(editorSpectatorName);

	scene->AddGameObject(editorSpectatorObject);

	GameObject* directionalLightObject = new GameObject();
	directionalLightObject->GetTransform()->SetRotationEulerDegrees(glm::vec3(90.0f, -45.0f, 0.0f));
	DirectionalLight* directionalLight = new DirectionalLight(glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
	directionalLightObject->AddComponent(directionalLight);

	std::string directionalLightName = "directional_light";
	directionalLightObject->SetName(directionalLightName);

	scene->AddGameObject(directionalLightObject);
}

void SceneExample_LitForward::Update()
{
	for (int i = 0; i < LAMPS_NUMBER; i++)
	{
		if (scene->GetSceneObjects()[lampObjectsIndices[i]] != nullptr)
		{
			glm::vec3 pointLightPosition = startPointLightPositions[i] + glm::vec3(0.0f, 1.0f, 0.0f) * sin(0.4f * ((GLfloat)i + 1.0f) * currentTime);
			scene->GetSceneObjects()[lampObjectsIndices[i]]->GetTransform()->SetPosition(pointLightPosition);
		}
	}

	SceneExample::Update();
}