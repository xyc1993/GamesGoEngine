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

	// Add sources of light
	constexpr int lightsNumber = 4;

	std::vector<glm::vec3> lightPositions;
	lightPositions.push_back(glm::vec3(0.9f, 0.0f, 0.9f));
	lightPositions.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	lightPositions.push_back(glm::vec3(-1.2f, 1.5f, -0.3f));
	lightPositions.push_back(glm::vec3(-0.8f, -0.8f, 0.4f));

	std::vector<glm::vec3> lightColors;
	lightColors.push_back(glm::vec3(9.0f, 0.0f, 0.0f));
	lightColors.push_back(glm::vec3(0.0f, 9.0f, 0.0f));
	lightColors.push_back(glm::vec3(0.0f, 0.0f, 15.0f));
	lightColors.push_back(glm::vec3(6.0f, 0.0f, 6.0f));

	for (int i = 0; i < lightsNumber; i++)
	{
		GameObject* pointLightObject = new GameObject();
		pointLightObject->GetTransform()->SetPosition(lightPositions[i]);
		pointLightObject->GetTransform()->SetScale(glm::vec3(0.2f));

		// Add debug renderer
		std::shared_ptr<Material> lampMaterial = std::make_shared<Material>("res/shaders/unlit.vert.glsl", "res/shaders/unlit.frag.glsl");
		lampMaterial->SetVector3((GLchar*)"unlitColor", lightColors[i]);
		MeshRenderer* lampMeshRenderer = new MeshRenderer();
		lampMeshRenderer->SetMesh(sphereMesh);
		lampMeshRenderer->SetMaterial(lampMaterial);
		lampMeshRenderer->SetIsCastingShadow(false);
		pointLightObject->AddComponent(lampMeshRenderer);

		// Add point light component
		PointLight* pointLight = new PointLight(0.02f * lightColors[i], 0.2f * lightColors[i], lightColors[i], 1.0f, 0.09f, 0.032f);
		pointLightObject->AddComponent(pointLight);

		// Name and add to scene
		std::string name = "point_light_";
		name.append(std::to_string(i));
		pointLightObject->SetName(name);

		scene->AddGameObject(pointLightObject);
	}
	
	AddEditorSpectator();
}