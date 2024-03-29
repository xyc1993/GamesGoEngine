#include "SceneExample_MultipleLightsShadows.h"

#include "Material.h"
#include "MeshPrimitivesPool.h"
#include "MeshRenderer.h"

namespace GamesGoEngine
{
	SceneExample_MultipleLightsShadows::SceneExample_MultipleLightsShadows()
	{
		std::shared_ptr<MeshPrimitiveQuad> quadMesh = MeshPrimitivesPool::GetQuadPrimitive();
		std::shared_ptr<MeshPrimitiveCube> cubeMesh = MeshPrimitivesPool::GetCubePrimitive();
		std::shared_ptr<MeshPrimitiveSphere> sphereMesh = MeshPrimitivesPool::GetSpherePrimitive();

		std::shared_ptr<Material> objectsMaterial = std::make_shared<Material>("res/shaders/litMultiShadowSimple.vert.glsl", "res/shaders/litMultiShadowSimple.frag.glsl");
		objectsMaterial->SetVector3("objectColor", glm::vec3(0.4f, 0.4f, 0.4f));
		objectsMaterial->SetLightModel(LightModelType::LitForward);

		// Create floor
		GameObject* floorObject = new GameObject();
		MeshRenderer* floorMeshRenderer = new MeshRenderer();
		floorMeshRenderer->SetMesh(quadMesh);
		floorMeshRenderer->SetMaterial(objectsMaterial);
		floorObject->AddComponent(floorMeshRenderer);
		std::string name = "floor";
		floorObject->SetName(name);
		floorObject->GetTransform()->SetPosition(glm::vec3(0.0f, -4.0f, 0.0f));
		floorObject->GetTransform()->SetRotationEulerDegrees(glm::vec3(-90.0f, 0.0f, 0.0f));
		floorObject->GetTransform()->SetScale(glm::vec3(15.0f));
		scene->AddGameObject(floorObject);


		// Create boxes
		const int BOXES_NUMBER = 3;

		glm::vec3 boxesPositions[BOXES_NUMBER] = {
			glm::vec3(3.0f, 0.0f, 0.0f),
			glm::vec3(2.0f, 5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f)
		};

		glm::vec3 boxesRotations[BOXES_NUMBER] = {
			glm::vec3(90.0f, 45.0f, 0.0f),
			glm::vec3(120.0f,60.0f, 0.0f),
			glm::vec3(150.0f,75.0f, 0.0f)
		};

		for (int i = 0; i < BOXES_NUMBER; i++)
		{
			GameObject* boxObject = new GameObject();
			MeshRenderer* boxMeshRenderer = new MeshRenderer();
			boxMeshRenderer->SetMesh(cubeMesh);
			boxMeshRenderer->SetMaterial(objectsMaterial);
			boxObject->AddComponent(boxMeshRenderer);
			std::string name = "box_";
			name.append(std::to_string(i));
			boxObject->SetName(name);
			boxObject->GetTransform()->SetPosition(boxesPositions[i]);
			boxObject->GetTransform()->SetRotationEulerDegrees(boxesRotations[i]);
			scene->AddGameObject(boxObject);
		}

		// Create spheres
		const int SPHERES_NUMBER = 3;

		glm::vec3 spheresPositions[SPHERES_NUMBER] = {
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f, 3.0f, -7.5f)
		};

		for (int i = 0; i < SPHERES_NUMBER; i++)
		{
			GameObject* sphereObject = new GameObject();
			MeshRenderer* sphereMeshRenderer = new MeshRenderer();
			sphereMeshRenderer->SetMesh(sphereMesh);
			sphereMeshRenderer->SetMaterial(objectsMaterial);
			sphereObject->AddComponent(sphereMeshRenderer);
			std::string name = "sphere_";
			name.append(std::to_string(i));
			sphereObject->SetName(name);
			sphereObject->GetTransform()->SetPosition(spheresPositions[i]);
			scene->AddGameObject(sphereObject);
		}

		// Create ambient light
		GameObject* ambientLightObject = new GameObject();
		AmbientLight* ambientLight = new AmbientLight(glm::vec3(0.03f, 0.03f, 0.005f));
		ambientLightObject->AddComponent(ambientLight);
		ambientLightObject->SetName("ambient_light");
		scene->AddGameObject(ambientLightObject);

		// Create directional light	
		GameObject* directionalLightObject = new GameObject();
		directionalLightObject->GetTransform()->SetRotationEulerDegrees(glm::vec3(135.0f, -45.0f, 0.0f));
		DirectionalLight* directionalLight = new DirectionalLight(glm::vec3(0.0f, 0.0f, 0.01f), glm::vec3(0.0f, 0.0f, 0.05f), glm::vec3(0.0f, 0.0f, 0.1f));
		directionalLightObject->AddComponent(directionalLight);
		std::string directionalLightName = "directional_light";
		directionalLightObject->SetName(directionalLightName);
		scene->AddGameObject(directionalLightObject);

		// Create point lights
		const int POINT_LIGHTS_NUMBER = 3;

		glm::vec3 pointLightsPositions[POINT_LIGHTS_NUMBER] = {
			glm::vec3(1.0f, 1.0f, -2.0f),
			glm::vec3(3.6f, 0.9f, -0.8f),
			glm::vec3(-1.7f, 4.0f, -7.5f)
		};

		glm::vec3 pointLightsColors[POINT_LIGHTS_NUMBER] = {
			glm::vec3(10.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 10.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 10.0f)
		};

		for (int i = 0; i < POINT_LIGHTS_NUMBER; i++)
		{
			GameObject* pointLightObject = new GameObject();
			pointLightObject->GetTransform()->SetPosition(pointLightsPositions[i]);
			pointLightObject->GetTransform()->SetScale(glm::vec3(0.2f));
			// Add debug renderer
			std::shared_ptr<Material> lampMaterial = std::make_shared<Material>("res/shaders/unlit.vert.glsl", "res/shaders/unlit.frag.glsl");
			glm::vec3 lightColor = pointLightsColors[i];
			lampMaterial->SetVector3((GLchar*)"unlitColor", lightColor);
			MeshRenderer* lampMeshRenderer = new MeshRenderer();
			lampMeshRenderer->SetMesh(sphereMesh);
			lampMeshRenderer->SetMaterial(lampMaterial);
			lampMeshRenderer->SetIsCastingShadow(false);
			pointLightObject->AddComponent(lampMeshRenderer);
			// Add point light component
			PointLight* pointLight = new PointLight(0.02f * lightColor, 0.8f * lightColor, lightColor, 5.0f, 3.2f, 1.2f, 10.0f, 8.0f);
			pointLightObject->AddComponent(pointLight);
			// Name and add to scene
			std::string name = "point_light_";
			name.append(std::to_string(i));
			pointLightObject->SetName(name);
			scene->AddGameObject(pointLightObject);
		}

		// Create spot lights
		const int SPOT_LIGHTS_NUMBER = 2;

		glm::vec3 spotLightsPositions[SPOT_LIGHTS_NUMBER] = {
			glm::vec3(1.0f, 0.1f, -2.0f),
			glm::vec3(4.2f, 0.1f, -0.5f)
		};

		glm::vec3 spotLightsRotations[SPOT_LIGHTS_NUMBER] = {
			glm::vec3(158.0f, -30.0f, 0.0f),
			glm::vec3(-34.0f,48.5f, 180.0f)
		};

		glm::vec3 spotLightsColors[SPOT_LIGHTS_NUMBER] = {
			glm::vec3(10.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 10.0f, 0.0f)
		};

		for (int i = 0; i < SPOT_LIGHTS_NUMBER; i++)
		{
			GameObject* spotLightObject = new GameObject();
			spotLightObject->GetTransform()->SetPosition(spotLightsPositions[i]);
			spotLightObject->GetTransform()->SetRotationEulerDegrees(spotLightsRotations[i]);
			spotLightObject->GetTransform()->SetScale(glm::vec3(0.2f));
			// Add debug renderer
			std::shared_ptr<Material> lampMaterial = std::make_shared<Material>("res/shaders/unlit.vert.glsl", "res/shaders/unlit.frag.glsl");
			glm::vec3 lightColor = spotLightsColors[i];
			lampMaterial->SetVector3((GLchar*)"unlitColor", lightColor);
			MeshRenderer* lampMeshRenderer = new MeshRenderer();
			lampMeshRenderer->SetMesh(cubeMesh);
			lampMeshRenderer->SetMaterial(lampMaterial);
			lampMeshRenderer->SetIsCastingShadow(false);
			spotLightObject->AddComponent(lampMeshRenderer);
			// Add point light component
			SpotLight* spotLight = new SpotLight(0.02f * lightColor, 0.8f * lightColor, lightColor, 5.0f, 3.2f, 1.2f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)));
			spotLightObject->AddComponent(spotLight);
			// Name and add to scene
			std::string name = "spot_light_";
			name.append(std::to_string(i));
			spotLightObject->SetName(name);
			scene->AddGameObject(spotLightObject);
		}

		AddEditorSpectator();
	}
}