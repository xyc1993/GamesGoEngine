#include "SceneExample_DeferredRendering.h"

#include "MeshPrimitiveCube.h"
#include "MeshPrimitiveSphere.h"
#include "MeshRenderer.h"

SceneExample_DeferredRendering::SceneExample_DeferredRendering()
{
	std::shared_ptr<MeshPrimitiveCube> cubeMesh = std::make_shared<MeshPrimitiveCube>();
	std::shared_ptr<MeshPrimitiveSphere> sphereMesh = std::make_shared<MeshPrimitiveSphere>();

	std::shared_ptr<Material> boxDeferredMaterial = std::make_shared<Material>("res/shaders/RenderPipeline/gBuffer.vert.glsl", "res/shaders/RenderPipeline/gBuffer.frag.glsl");
	boxDeferredMaterial->SetTextureByPath((GLchar*)"texture_diffuse1", 0, (GLchar*)"res/box/container2_diffuse.png");
	boxDeferredMaterial->SetTextureByPath((GLchar*)"texture_specular1", 1, (GLchar*)"res/box/container2_specular.png");
	boxDeferredMaterial->SetLightModel(LightModelType::LitDeferred);

	std::vector<glm::vec3> boxPositions;
	boxPositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	boxPositions.push_back(glm::vec3(0.0f, -2.0f, 0.0f));
	boxPositions.push_back(glm::vec3(0.0f, 6.0f, 0.0f));
	boxPositions.push_back(glm::vec3(-7.0f, 0.0f, 0.0f));
	boxPositions.push_back(glm::vec3(7.0f, 0.0f, 0.0f));
	boxPositions.push_back(glm::vec3(0.0f, 0.0f, -7.0f));
	boxPositions.push_back(glm::vec3(0.0f, 0.0f, 7.0f));

	for (size_t i = 0; i < boxPositions.size(); i++)
	{
		GameObject* boxObject = new GameObject();
		MeshRenderer* boxMeshRenderer = new MeshRenderer();
		boxMeshRenderer->SetMesh(cubeMesh);
		boxMeshRenderer->SetMaterial(boxDeferredMaterial);
		boxMeshRenderer->SetIsCastingShadow(false);

		boxObject->AddComponent(boxMeshRenderer);
		std::string name = "box_";
		name.append(std::to_string(i));
		boxObject->SetName(name);
		boxObject->GetTransform()->SetPosition(boxPositions[i]);

		scene->AddGameObject(boxObject);
	}

	// Add source of light
	GameObject* pointLightObject = new GameObject();
	pointLightObject->GetTransform()->SetPosition(glm::vec3(0.9f, 0.0f, 0.9f));
	pointLightObject->GetTransform()->SetScale(glm::vec3(0.2f));
	// Add debug renderer
	glm::vec3 lightColor = glm::vec3(10.0f, 8.0f, 6.0f);
	/*
	std::shared_ptr<Material> lampMaterial = std::make_shared<Material>("res/shaders/unlit.vert.glsl", "res/shaders/unlit.frag.glsl");	
	lampMaterial->SetVector3((GLchar*)"unlitColor", lightColor);
	MeshRenderer* lampMeshRenderer = new MeshRenderer();
	lampMeshRenderer->SetMesh(sphereMesh);
	lampMeshRenderer->SetMaterial(lampMaterial);
	lampMeshRenderer->SetIsCastingShadow(false);
	pointLightObject->AddComponent(lampMeshRenderer);
	*/
	// Add point light component
	PointLight* pointLight = new PointLight(0.02f * lightColor, 0.8f * lightColor, lightColor, 1.0f, 0.09f, 0.032f);
	pointLightObject->AddComponent(pointLight);
	// Name and add to scene
	pointLightObject->SetName("point_light");
	scene->AddGameObject(pointLightObject);

	AddEditorSpectator();
}