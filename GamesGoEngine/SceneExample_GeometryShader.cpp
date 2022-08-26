#include "SceneExample_GeometryShader.h"

#include "MeshImported.h"
#include "MeshRenderer.h"
#include "TextureLoader.h"

SceneExample_GeometryShader::SceneExample_GeometryShader()
{
	std::vector<const GLchar*> skyboxTextures;
	skyboxTextures.push_back("res/textures/skybox/right.tga");
	skyboxTextures.push_back("res/textures/skybox/left.tga");
	skyboxTextures.push_back("res/textures/skybox/top.tga");
	skyboxTextures.push_back("res/textures/skybox/bottom.tga");
	skyboxTextures.push_back("res/textures/skybox/back.tga");
	skyboxTextures.push_back("res/textures/skybox/front.tga");

	GLuint skyboxTexture = TextureLoader::LoadCubemap(skyboxTextures);

	std::shared_ptr<Material> skyboxMaterial = std::make_shared<Material>("res/shaders/skybox.vert.glsl", "res/shaders/skybox.frag.glsl");
	skyboxMaterial->SetCubeTexture("skybox", 1, skyboxTexture);

	SkyboxRenderer* skyboxComponent = new SkyboxRenderer();
	skyboxComponent->SetMaterial(skyboxMaterial);

	GameObject* skyboxObject = new GameObject();
	skyboxObject->AddComponent(skyboxComponent);

	std::string skyboxName = "skybox";
	skyboxObject->SetName(skyboxName);

	scene->AddGameObject(skyboxObject);

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
		nanoSuitMaterials[i] = std::make_shared<Material>("res/shaders/explode.vert.glsl", "res/shaders/unlitTextured.frag.glsl", "res/shaders/explode.geom.glsl");
		nanoSuitMaterials[i]->SetTextureByPath((GLchar*)"mainTexture", 0, nanoSuitTexturePaths[i]);
		nanoSuitMeshRenderer->SetMaterial(nanoSuitMaterials[i], i);
	}

	nanoSuitObject->AddComponent(nanoSuitMeshRenderer);

	std::string nanoSuitName = "nanosuit";
	nanoSuitObject->SetName(nanoSuitName);

	scene->AddGameObject(nanoSuitObject);

	GameObject* editorSpectatorObject = AddEditorSpectator();
}