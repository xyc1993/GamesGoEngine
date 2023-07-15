#include "SceneExample_LitForward.h"

#include "Camera.h"
#include "DirectionalLight.h"
#include "EditorMovement.h"
#include "Material.h"
#include "MeshImported.h"
#include "MeshPrimitiveCube.h"
#include "MeshPrimitiveQuad.h"
#include "MeshPrimitiveSphere.h"
#include "MeshRenderer.h"
#include "PointLight.h"
#include "PositionOscillator.h"
#include "PostProcessRenderer.h"
#include "Rotator.h"
#include "ScaleOscillator.h"
#include "SkyboxRenderer.h"
#include "SpotLight.h"

SceneExample_LitForward::SceneExample_LitForward()
{
	InitScene(false);
}

SceneExample_LitForward::SceneExample_LitForward(bool shouldAddPostProcessEffects)
{
	InitScene(shouldAddPostProcessEffects);
}

void SceneExample_LitForward::InitScene(bool shouldAddPostProcessEffects)
{
	const int LIT_BOXES_NUMBER = 10;

	glm::vec3 litBoxesPositions[LIT_BOXES_NUMBER] = {
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

	glm::vec3 litBoxesRotations[LIT_BOXES_NUMBER] = {
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

	const int LAMPS_NUMBER = 4;

	glm::vec3 startPointLightPositions[LAMPS_NUMBER] = {
		glm::vec3(0.07,   0.2f,   1.0f),
		glm::vec3(2.3f,   -3.3f,   -4.0f),
		glm::vec3(-4.0f,  -2.0f,  -11.0f),
		glm::vec3(0.0f,  0.0f,  -3.0f)
	};

	std::vector<const GLchar*> skyboxTextures;
	skyboxTextures.push_back("res/textures/skybox/right.tga");
	skyboxTextures.push_back("res/textures/skybox/left.tga");
	skyboxTextures.push_back("res/textures/skybox/top.tga");
	skyboxTextures.push_back("res/textures/skybox/bottom.tga");
	skyboxTextures.push_back("res/textures/skybox/back.tga");
	skyboxTextures.push_back("res/textures/skybox/front.tga");

	std::shared_ptr<Material> skyboxMaterial = std::make_shared<Material>("res/shaders/skybox.vert.glsl", "res/shaders/skybox.frag.glsl");
	skyboxMaterial->SetCubeTextureByPath("skybox", 0, skyboxTextures);

	SkyboxRenderer* skyboxComponent = new SkyboxRenderer();
	skyboxComponent->SetMaterial(skyboxMaterial);

	GameObject* skyboxObject = new GameObject();
	skyboxObject->AddComponent(skyboxComponent);

	std::string skyboxName = "skybox";
	skyboxObject->SetName(skyboxName);

	scene->AddGameObject(skyboxObject);

	std::shared_ptr<MeshPrimitiveCube> cubeMesh = std::make_shared<MeshPrimitiveCube>();
	//std::shared_ptr<MeshPrimitiveQuad> quadMesh = std::make_shared<MeshPrimitiveQuad>();
	std::shared_ptr<MeshPrimitiveSphere> sphereMesh = std::make_shared<MeshPrimitiveSphere>();

	const glm::vec3 lightsColor = glm::vec3(10.0f);

	std::shared_ptr<Material> lampMaterial = std::make_shared<Material>("res/shaders/unlit.vert.glsl", "res/shaders/unlit.frag.glsl");
	lampMaterial->SetVector3((GLchar*)"unlitColor", glm::vec3(lightsColor));

	GameObject* lampParent = nullptr;
	for (int i = 0; i < LAMPS_NUMBER; i++)
	{
		GameObject* lampObject = new GameObject();
		MeshRenderer* lampMeshRenderer = new MeshRenderer();
		lampMeshRenderer->SetMesh(sphereMesh);
		lampMeshRenderer->SetMaterial(lampMaterial);
		lampObject->AddComponent(lampMeshRenderer);
		lampObject->GetTransform()->SetPosition(startPointLightPositions[i]);
		lampObject->GetTransform()->SetScale(glm::vec3(0.2f));

		PointLight* pointLight = new PointLight(0.05f * lightsColor, 0.8f * lightsColor, 1.0f * lightsColor, 1.0f, 0.09f, 0.032f);
		lampObject->AddComponent(pointLight);

		PositionOscillator* oscillator = new PositionOscillator();
		oscillator->SetAmplitude(glm::vec3(0.0f, 1.0f, 0.0f));
		oscillator->SetSpeed(0.4f + (float)i);
		lampObject->AddComponent(oscillator);

		std::string name = "sphere_lamp_";
		name.append(std::to_string(i));
		lampObject->SetName(name);

		scene->AddGameObject(lampObject);
		if (i == 0) lampParent = lampObject;
	}

	std::shared_ptr<Material> cubeLitMaterial = std::make_shared<Material>("res/shaders/lighting.vert.glsl", "res/shaders/lighting.frag.glsl");
	cubeLitMaterial->SetTextureByPath((GLchar*)"material.diffuse", 0, (GLchar*)"res/box/container2_diffuse.png");
	cubeLitMaterial->SetTextureByPath((GLchar*)"material.specular", 1, (GLchar*)"res/box/container2_specular.png");
	cubeLitMaterial->SetFloat((GLchar*)"material.shininess", 32.0f);
	cubeLitMaterial->SetLightModel(LightModelType::LitForward);

	GameObject* boxChild1 = nullptr;
	GameObject* boxChild2 = nullptr;
	GameObject* boxChild3 = nullptr;
	for (int i = 0; i < LIT_BOXES_NUMBER; i++)
	{
		GameObject* litBoxesObject = new GameObject();
		MeshRenderer* litBoxesMeshRenderer = new MeshRenderer();
		litBoxesMeshRenderer->SetMesh(cubeMesh);
		litBoxesMeshRenderer->SetMaterial(cubeLitMaterial);
		litBoxesObject->AddComponent(litBoxesMeshRenderer);
		litBoxesObject->GetTransform()->SetPosition(litBoxesPositions[i]);
		litBoxesObject->GetTransform()->SetRotationEulerDegrees(litBoxesRotations[i]);

		std::string name = "lit_box_";
		name.append(std::to_string(i));
		litBoxesObject->SetName(name);

		scene->AddGameObject(litBoxesObject);
		if (i == 0)
		{
			boxChild1 = litBoxesObject;
		}
		if (i == 1)
		{
			boxChild2 = litBoxesObject;
		}
		if (i == 3)
		{
			boxChild3 = litBoxesObject;
		}
	}

	if (boxChild1 != nullptr && lampParent != nullptr && boxChild2 != nullptr && boxChild3 != nullptr)
	{
		boxChild1->SetParent(lampParent);
		Rotator* rotator = new Rotator();
		rotator->SetSpeed(glm::vec3(0.0f, 20.0f, 0.0f));
		boxChild1->AddComponent(rotator);

		boxChild2->SetParent(boxChild1);
		ScaleOscillator* scaleOscillator = new ScaleOscillator();
		scaleOscillator->SetAmplitude(glm::vec3(0.5f));
		scaleOscillator->SetSpeed(2.0f);
		boxChild2->AddComponent(scaleOscillator);

		boxChild3->SetParent(boxChild2);
		PositionOscillator* positionOscillator = new PositionOscillator();
		positionOscillator->SetAmplitude(glm::vec3(4.0f, 0.0f, 0.0f));
		positionOscillator->SetSpeed(3.2f);
		boxChild3->AddComponent(positionOscillator);
	}

	GameObject* nanoSuitObject = new GameObject();
	nanoSuitObject->GetTransform()->SetPosition(glm::vec3(0.0f, -1.75f, 0.0f));
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
		nanoSuitMaterials[i] = std::make_shared<Material>("res/shaders/lighting.vert.glsl", "res/shaders/lighting.frag.glsl");
		nanoSuitMaterials[i]->SetTextureByPath((GLchar*)"material.diffuse", 0, nanoSuitTexturePaths[i]);
		nanoSuitMaterials[i]->SetFloat((GLchar*)"material.shininess", 32.0f);
		nanoSuitMaterials[i]->SetLightModel(LightModelType::LitForward);
		nanoSuitMeshRenderer->SetMaterial(nanoSuitMaterials[i], i);
	}

	nanoSuitObject->AddComponent(nanoSuitMeshRenderer);

	std::string nanoSuitName = "nanosuit";
	nanoSuitObject->SetName(nanoSuitName);

	scene->AddGameObject(nanoSuitObject);

	if (shouldAddPostProcessEffects)
	{
		GameObject* postProcessHolder = new GameObject;

		std::shared_ptr<PostProcessMaterial> ppMaterial1 = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/inverseColor.frag.glsl");
		std::shared_ptr<PostProcessMaterial> ppMaterial2 = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/blur.frag.glsl");

		PostProcessRenderer* ppr = new PostProcessRenderer();

		ppr->AddMaterial(ppMaterial1);
		ppr->AddMaterial(ppMaterial2);

		postProcessHolder->AddComponent(ppr);

		std::string postProcessHolderName = "post_process_holder";
		postProcessHolder->SetName(postProcessHolderName);

		scene->AddGameObject(postProcessHolder);
	}
	
	GameObject* directionalLightObject = new GameObject();
	directionalLightObject->GetTransform()->SetRotationEulerDegrees(glm::vec3(90.0f, -45.0f, 0.0f));
	DirectionalLight* directionalLight = new DirectionalLight(glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
	directionalLightObject->AddComponent(directionalLight);

	std::string directionalLightName = "directional_light";
	directionalLightObject->SetName(directionalLightName);

	scene->AddGameObject(directionalLightObject);

	GameObject* editorSpectatorObject = AddEditorSpectator();
	SpotLight* spotLight = new SpotLight(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f),
		1.0f, 0.09f, 0.032f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)));
	editorSpectatorObject->AddComponent(spotLight);
}