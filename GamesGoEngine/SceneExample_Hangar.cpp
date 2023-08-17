#include "SceneExample_Hangar.h"

#include "MeshRenderer.h"
#include "MeshImported.h"
#include "MeshPrimitivesPool.h"
#include "PositionOscillator.h"

SceneExample_Hangar::SceneExample_Hangar()
{
	GameObject* hangarObject = new GameObject();
	hangarObject->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, -3.0f));
	hangarObject->GetTransform()->SetRotationEulerDegrees(glm::vec3(0.0f, 180.0f, 0.0f));
	MeshRenderer* hangarMeshRenderer = new MeshRenderer();
	std::shared_ptr<MeshImported> hangarMesh = std::make_shared<MeshImported>((GLchar*)"res/hangar/scifiscene.obj");
	hangarMeshRenderer->SetMesh(hangarMesh);

	// set materials, this model accepts 2 materials
	std::vector<std::shared_ptr<Material>> hangarMaterials;

	std::shared_ptr<Material> hangarBaseDeferredMaterial = std::make_shared<Material>("res/shaders/RenderPipeline/gBufferPBR.vert.glsl", "res/shaders/RenderPipeline/gBufferPBR.frag.glsl");
	hangarBaseDeferredMaterial->SetTextureByPath((GLchar*)"albedoMap", 0, (GLchar*)"res/hangar/Base_Base_color.png");
	hangarBaseDeferredMaterial->SetTextureByPath((GLchar*)"normalMap", 1, (GLchar*)"res/hangar/Base_Normal_DirectX.png", false, false);
	hangarBaseDeferredMaterial->SetTextureByPath((GLchar*)"metallicMap", 2, (GLchar*)"res/hangar/Base_Metallic.png",false, false);
	hangarBaseDeferredMaterial->SetTextureByPath((GLchar*)"roughnessMap", 3, (GLchar*)"res/hangar/Base_Roughness.png", false, false);
	hangarBaseDeferredMaterial->SetTextureByPath((GLchar*)"aoMap", 4, (GLchar*)"res/hangar/Base_Mixed_AO.png", false, false);
	hangarBaseDeferredMaterial->SetTextureByPath((GLchar*)"emissiveMap", 5, (GLchar*)"res/hangar/Base_Emissive.png");
	hangarBaseDeferredMaterial->SetFloat("emissiveStrength", 1.5f);
	hangarBaseDeferredMaterial->SetLightModel(LightModelType::LitDeferred);
	hangarMaterials.push_back(hangarBaseDeferredMaterial);

	std::shared_ptr<Material> hangarDetailsDeferredMaterial = std::make_shared<Material>("res/shaders/RenderPipeline/gBufferPBR.vert.glsl", "res/shaders/RenderPipeline/gBufferPBR.frag.glsl");
	hangarDetailsDeferredMaterial->SetTextureByPath((GLchar*)"albedoMap", 0, (GLchar*)"res/hangar/Details_Cevre_Base_color.png");
	hangarDetailsDeferredMaterial->SetTextureByPath((GLchar*)"normalMap", 1, (GLchar*)"res/hangar/Details_Cevre_Normal_DirectX.png", false, false);
	hangarDetailsDeferredMaterial->SetTextureByPath((GLchar*)"metallicMap", 2, (GLchar*)"res/hangar/Details_Cevre_Metallic.png", false, false);
	hangarDetailsDeferredMaterial->SetTextureByPath((GLchar*)"roughnessMap", 3, (GLchar*)"res/hangar/Details_Cevre_Roughness.png", false, false);
	hangarDetailsDeferredMaterial->SetTextureByPath((GLchar*)"aoMap", 4, (GLchar*)"res/hangar/Details_Cevre_Mixed_AO.png", false, false);
	hangarDetailsDeferredMaterial->SetTextureByPath((GLchar*)"emissiveMap", 5, (GLchar*)"res/hangar/Details_Cevre_Emissive.png");
	hangarDetailsDeferredMaterial->SetFloat("emissiveStrength", 1.5f);
	hangarDetailsDeferredMaterial->SetLightModel(LightModelType::LitDeferred);
	hangarMaterials.push_back(hangarDetailsDeferredMaterial);

	for (int i = 0; i < hangarMesh->GetSubMeshesCount(); i++)
	{
		hangarMeshRenderer->SetMaterial(hangarMaterials[i], i);
	}

	hangarObject->AddComponent(hangarMeshRenderer);

	std::string hangarName = "hangar";
	hangarObject->SetName(hangarName);

	scene->AddGameObject(hangarObject);

	// Add ambient light
	glm::vec3 ambientLightColor = glm::vec3(0.1f, 0.1f, 0.1f);
	GameObject* ambientLightObject = new GameObject();
	AmbientLight* ambientLight = new AmbientLight(ambientLightColor);
	ambientLightObject->AddComponent(ambientLight);
	ambientLightObject->SetName("ambient_light");
	scene->AddGameObject(ambientLightObject);

	// Add point light
	std::shared_ptr<MeshPrimitiveSphere> sphereMesh = MeshPrimitivesPool::GetSpherePrimitive();
	glm::vec3 pointLightColor = glm::vec3(5.0f, 5.0f, 2.8f);
	GameObject* pointLightObject = new GameObject();
	pointLightObject->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, -1.5f));
	pointLightObject->GetTransform()->SetScale(glm::vec3(0.05f));

	// Add debug renderer
	std::shared_ptr<Material> lampMaterial = std::make_shared<Material>("res/shaders/RenderPipeline/gBuffer.vert.glsl", "res/shaders/RenderPipeline/gBufferEmissive.frag.glsl");
	lampMaterial->SetVector3((GLchar*)"emissiveColor", pointLightColor);
	lampMaterial->SetFloat("emissiveStrength", 1.0f);
	lampMaterial->SetLightModel(LightModelType::LitDeferred);
	MeshRenderer* lampMeshRenderer = new MeshRenderer();
	lampMeshRenderer->SetMesh(sphereMesh);
	lampMeshRenderer->SetMaterial(lampMaterial);
	lampMeshRenderer->SetIsCastingShadow(false);
	pointLightObject->AddComponent(lampMeshRenderer);

	// Add point light component
	PointLight* pointLight = new PointLight(0.02f * pointLightColor, 0.2f * pointLightColor, pointLightColor, 3.1f, 0.9f, 0.32f, 10.0f, 8.0f);
	pointLightObject->AddComponent(pointLight);

	// Add oscillator component
	PositionOscillator* oscillator = new PositionOscillator();
	oscillator->SetAmplitude(glm::vec3(0.0f, 0.0f, 2.5f));
	oscillator->SetSpeed(1.8f);
	pointLightObject->AddComponent(oscillator);

	// Name and add to scene
	std::string name = "point_light";
	pointLightObject->SetName(name);

	scene->AddGameObject(pointLightObject);

	AddEditorSpectator();
}
