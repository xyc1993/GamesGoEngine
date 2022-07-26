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

	std::shared_ptr<Material> skyboxMaterial = std::make_shared<Material>("res/shaders/skybox.vert.glsl", "res/shaders/skybox.frag.glsl");
	skyboxMaterial->SetCubeTextureByPath("skybox", 1, skyboxTextures);

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

	GLuint skullDiffuseTexture = TextureLoader::LoadTexture((GLchar*)"res/skull/diffuse.jpg", false);
	
	for (int i = 0; i < skullMesh->GetSubMeshesCount(); i++)
	{
		std::shared_ptr<Material> skullMaterial = std::make_shared<Material>("res/shaders/lighting.vert.glsl", "res/shaders/lighting.frag.glsl");
		skullMaterial->SetTexture((GLchar*)"material.diffuse", 0, skullDiffuseTexture);
		skullMaterial->SetFloat((GLchar*)"material.shininess", 32.0f);
		skullMaterial->SetLightModel(LightModelType::LitForward);
		skullMeshRenderer->SetMaterial(skullMaterial, i);
	}

	skullObject->AddComponent(skullMeshRenderer);

	std::string skullName = "skull";
	skullObject->SetName(skullName);

	scene->AddGameObject(skullObject);

	GameObject* directionalLightObject = new GameObject();
	directionalLightObject->GetTransform()->SetRotationEulerDegrees(glm::vec3(90.0f, -45.0f, 0.0f));
	DirectionalLight* directionalLight = new DirectionalLight(glm::vec3(0.2f), glm::vec3(0.6f), glm::vec3(0.7f));
	directionalLightObject->AddComponent(directionalLight);

	std::string directionalLightName = "directional_light";
	directionalLightObject->SetName(directionalLightName);

	scene->AddGameObject(directionalLightObject);

	GameObject* editorSpectatorObject = AddEditorSpectator();
}