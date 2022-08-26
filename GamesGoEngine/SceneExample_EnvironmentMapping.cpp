#include "SceneExample_EnvironmentMapping.h"

#include "MeshImported.h"
#include "MeshRenderer.h"
#include "TextureLoader.h"

SceneExample_EnvironmentMapping::SceneExample_EnvironmentMapping()
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

	GameObject* skullObject = new GameObject();
	skullObject->GetTransform()->SetPosition(glm::vec3(0.0f, -2.0f, -5.0f));
	skullObject->GetTransform()->SetRotationEulerDegrees(glm::vec3(-90.0f, 0.0f, 0.0f));
	skullObject->GetTransform()->SetScale(glm::vec3(0.2f));

	MeshRenderer* skullMeshRenderer = new MeshRenderer();
	std::shared_ptr<MeshImported> skullMesh = std::make_shared<MeshImported>((GLchar*)"res/skull/skull.obj");
	skullMeshRenderer->SetMesh(skullMesh);
	
	for (int i = 0; i < skullMesh->GetSubMeshesCount(); i++)
	{
		std::shared_ptr<Material> skullMaterial = std::make_shared<Material>("res/shaders/skyboxReflect.vert.glsl", "res/shaders/skyboxReflect.frag.glsl");
		skullMaterial->SetCubeTexture((GLchar*)"skybox", 1, skyboxTexture);
		skullMeshRenderer->SetMaterial(skullMaterial, i);
	}

	skullObject->AddComponent(skullMeshRenderer);

	std::string skullName = "skull";
	skullObject->SetName(skullName);

	scene->AddGameObject(skullObject);

	GameObject* editorSpectatorObject = AddEditorSpectator();
}