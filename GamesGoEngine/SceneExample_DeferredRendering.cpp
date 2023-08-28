#include "SceneExample_DeferredRendering.h"

#include "MeshPrimitivesPool.h"
#include "MeshRenderer.h"

namespace GamesGoEngine
{
	SceneExample_DeferredRendering::SceneExample_DeferredRendering()
	{
		std::shared_ptr<MeshPrimitiveCube> cubeMesh = MeshPrimitivesPool::GetCubePrimitive();
		std::shared_ptr<MeshPrimitiveQuad> quadMesh = MeshPrimitivesPool::GetQuadPrimitive();
		std::shared_ptr<MeshPrimitiveSphere> sphereMesh = MeshPrimitivesPool::GetSpherePrimitive();

		std::shared_ptr<Material> boxDeferredMaterial = std::make_shared<Material>("res/shaders/RenderPipeline/gBuffer.vert.glsl", "res/shaders/RenderPipeline/gBuffer.frag.glsl");
		boxDeferredMaterial->SetTextureByPath((GLchar*)"texture_diffuse1", 0, (GLchar*)"res/box/container2_diffuse.png");
		boxDeferredMaterial->SetTextureByPath((GLchar*)"texture_specular1", 1, (GLchar*)"res/box/container2_specular.png");
		boxDeferredMaterial->SetLightModel(LightModelType::LitDeferred);

		std::shared_ptr<Material> bricksDeferredMaterial = std::make_shared<Material>("res/shaders/RenderPipeline/gBuffer.vert.glsl", "res/shaders/RenderPipeline/gBuffer.frag.glsl");
		bricksDeferredMaterial->SetTextureByPath((GLchar*)"texture_diffuse1", 0, (GLchar*)"res/textures/brickDiffNormSpec/brick_wall2-diff-1024.tga");
		bricksDeferredMaterial->SetTextureByPath((GLchar*)"texture_specular1", 1, (GLchar*)"res/textures/brickDiffNormSpec/brick_wall2-spec-1024.tga");
		bricksDeferredMaterial->SetLightModel(LightModelType::LitDeferred);

		std::vector<glm::vec3> boxPositions;
		boxPositions.push_back(glm::vec3(0.0f, 1.5f, 0.0f));
		boxPositions.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
		boxPositions.push_back(glm::vec3(0.0f, 4.0f, 0.0f));
		boxPositions.push_back(glm::vec3(-5.0f, 1.5f, 0.0f));
		boxPositions.push_back(glm::vec3(5.0f, 1.5f, 0.0f));
		boxPositions.push_back(glm::vec3(0.0f, 1.5f, -5.0f));
		boxPositions.push_back(glm::vec3(0.0f, 1.5f, 5.0f));

		glm::vec3 boxRotations[7] = {
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(30.0f, 15.0f, 0.0f),
			glm::vec3(60.0f, 30.0f, 0.0f),
			glm::vec3(90.0f, 45.0f, 0.0f),
			glm::vec3(120.0f,60.0f, 0.0f),
			glm::vec3(150.0f,75.0f, 0.0f),
			glm::vec3(180.0f,90.0f, 0.0f),
		};

		for (size_t i = 0; i < boxPositions.size(); i++)
		{
			GameObject* boxObject = new GameObject();
			MeshRenderer* boxMeshRenderer = new MeshRenderer();
			boxMeshRenderer->SetMesh(cubeMesh);
			boxMeshRenderer->SetMaterial(boxDeferredMaterial);

			boxObject->AddComponent(boxMeshRenderer);
			std::string name = "box_";
			name.append(std::to_string(i));
			boxObject->SetName(name);
			boxObject->GetTransform()->SetPosition(boxPositions[i]);
			boxObject->GetTransform()->SetRotationEulerDegrees(boxRotations[i]);

			scene->AddGameObject(boxObject);
		}

		// Add walls, floor and ceiling
		std::vector<glm::vec3> boundsPositions;
		boundsPositions.push_back(glm::vec3(0.0f, -2.0f, 0.0f));
		boundsPositions.push_back(glm::vec3(-7.0f, 2.0f, 0.0f));
		boundsPositions.push_back(glm::vec3(7.0f, 2.0f, 0.0f));
		boundsPositions.push_back(glm::vec3(0.0f, 2.0f, -7.0f));
		boundsPositions.push_back(glm::vec3(0.0f, 2.0f, 7.0f));
		//outer walls for the shading and better orientation in the void
		boundsPositions.push_back(glm::vec3(-7.1f, 2.0f, 0.0f));
		boundsPositions.push_back(glm::vec3(7.1f, 2.0f, 0.0f));
		boundsPositions.push_back(glm::vec3(0.0f, 2.0f, -7.1f));
		boundsPositions.push_back(glm::vec3(0.0f, 2.0f, 7.1f));

		std::vector<glm::vec3> boundsRotations;
		boundsRotations.push_back(glm::vec3(-90.0f, 0.0f, 0.0f));
		boundsRotations.push_back(glm::vec3(0.0f, 90.0f, 0.0f));
		boundsRotations.push_back(glm::vec3(0.0f, -90.0f, 0.0f));
		boundsRotations.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		boundsRotations.push_back(glm::vec3(0.0f, 180.0f, 0.0f));
		//outer walls for the shading and better orientation in the void
		boundsRotations.push_back(glm::vec3(0.0f, 270.0f, 0.0f));
		boundsRotations.push_back(glm::vec3(0.0f, 90.0f, 0.0f));
		boundsRotations.push_back(glm::vec3(0.0f, 180.0f, 0.0f));
		boundsRotations.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

		for (size_t i = 0; i < boundsPositions.size(); i++)
		{
			GameObject* boundObject = new GameObject();
			MeshRenderer* boundMeshRenderer = new MeshRenderer();
			boundMeshRenderer->SetMesh(quadMesh);

			std::string name = "bound_";

			boundMeshRenderer->SetMaterial(bricksDeferredMaterial);
			boundObject->AddComponent(boundMeshRenderer);
			boundObject->GetTransform()->SetPosition(boundsPositions[i]);
			boundObject->GetTransform()->SetRotationEulerDegrees(boundsRotations[i]);
			if (i < 1) // floor
			{
				boundObject->GetTransform()->SetScale(glm::vec3(7.0f));
			}
			else if (i < 5) // inner walls
			{
				boundObject->GetTransform()->SetScale(glm::vec3(7.0f, 4.0f, 7.0f));
			}
			else // outer walls
			{
				boundObject->GetTransform()->SetScale(glm::vec3(7.1f, 4.0f, 7.0f));
			}

			name.append(std::to_string(i));
			boundObject->SetName(name);

			scene->AddGameObject(boundObject);
		}

		// Add ambient light
		glm::vec3 ambientLightColor = glm::vec3(0.0f, 0.008f, 0.008f);
		GameObject* ambientLightObject = new GameObject();
		AmbientLight* ambientLight = new AmbientLight(ambientLightColor);
		ambientLightObject->AddComponent(ambientLight);
		ambientLightObject->SetName("ambient_light");
		scene->AddGameObject(ambientLightObject);

		// Add directional light
		glm::vec3 directionalLightColor = glm::vec3(0.0f, 0.0f, 3.0f);
		GameObject* directionalLightObject = new GameObject();
		directionalLightObject->GetTransform()->SetRotationEulerDegrees(glm::vec3(135.0f, -45.0f, 0.0f));
		DirectionalLight* directionalLight = new DirectionalLight(0.02f * directionalLightColor, 0.05f * directionalLightColor, 0.08f * directionalLightColor);
		directionalLightObject->AddComponent(directionalLight);
		std::string directionalLightName = "directional_light";
		directionalLightObject->SetName(directionalLightName);
		scene->AddGameObject(directionalLightObject);

		// Add point lights
		constexpr int pointLightsNumber = 2;

		std::vector<glm::vec3> pointLightPositions;
		pointLightPositions.push_back(glm::vec3(-5.0f, 1.5f, -2.9f));
		pointLightPositions.push_back(glm::vec3(2.8f, 0.5f, 2.9f));

		std::vector<glm::vec3> pointLightColors;
		pointLightColors.push_back(glm::vec3(9.0f, 0.0f, 0.0f));
		pointLightColors.push_back(glm::vec3(0.0f, 9.0f, 0.0f));

		for (int i = 0; i < pointLightsNumber; i++)
		{
			GameObject* pointLightObject = new GameObject();
			pointLightObject->GetTransform()->SetPosition(pointLightPositions[i]);
			pointLightObject->GetTransform()->SetScale(glm::vec3(0.2f));

			// Add debug renderer
			std::shared_ptr<Material> lampMaterial = std::make_shared<Material>("res/shaders/RenderPipeline/gBuffer.vert.glsl", "res/shaders/RenderPipeline/gBufferEmissive.frag.glsl");
			lampMaterial->SetVector3((GLchar*)"emissiveColor", pointLightColors[i]);
			lampMaterial->SetFloat("emissiveStrength", 1.0f);
			lampMaterial->SetLightModel(LightModelType::LitDeferred);
			MeshRenderer* lampMeshRenderer = new MeshRenderer();
			lampMeshRenderer->SetMesh(sphereMesh);
			lampMeshRenderer->SetMaterial(lampMaterial);
			lampMeshRenderer->SetIsCastingShadow(false);
			pointLightObject->AddComponent(lampMeshRenderer);

			// Add point light component
			PointLight* pointLight = new PointLight(0.02f * pointLightColors[i], 0.2f * pointLightColors[i], pointLightColors[i], 3.1f, 0.9f, 0.32f, 10.0f, 8.0f);
			pointLightObject->AddComponent(pointLight);

			// Name and add to scene
			std::string name = "point_light_";
			name.append(std::to_string(i));
			pointLightObject->SetName(name);

			scene->AddGameObject(pointLightObject);
		}

		// Add spotlights
		constexpr int spotLightsNumber = 2;

		std::vector<glm::vec3> spotLightPositions;
		spotLightPositions.push_back(glm::vec3(-3.2f, 3.0f, -2.8f));
		spotLightPositions.push_back(glm::vec3(-1.7f, 1.0f, 3.1f));

		std::vector<glm::vec3> spotLightRotations;
		spotLightRotations.push_back(glm::vec3(23.3f, -40.5f, 0.0f));
		spotLightRotations.push_back(glm::vec3(-18.6f, 24.3f, 0.0f));

		std::vector<glm::vec3> spotLightColors;
		spotLightColors.push_back(glm::vec3(0.0f, 0.0f, 25.0f));
		spotLightColors.push_back(glm::vec3(11.0f, 0.0f, 11.0f));

		for (int i = 0; i < spotLightsNumber; i++)
		{
			GameObject* spotLightObject = new GameObject();
			spotLightObject->GetTransform()->SetPosition(spotLightPositions[i]);
			spotLightObject->GetTransform()->SetRotationEulerDegrees(spotLightRotations[i]);
			spotLightObject->GetTransform()->SetScale(glm::vec3(0.2f));

			// Add debug renderer
			std::shared_ptr<Material> lampMaterial = std::make_shared<Material>("res/shaders/RenderPipeline/gBuffer.vert.glsl", "res/shaders/RenderPipeline/gBufferEmissive.frag.glsl");
			lampMaterial->SetVector3((GLchar*)"emissiveColor", spotLightColors[i]);
			lampMaterial->SetFloat("emissiveStrength", 1.0f);
			lampMaterial->SetLightModel(LightModelType::LitDeferred);
			MeshRenderer* lampMeshRenderer = new MeshRenderer();
			lampMeshRenderer->SetMesh(cubeMesh);
			lampMeshRenderer->SetMaterial(lampMaterial);
			lampMeshRenderer->SetIsCastingShadow(false);
			spotLightObject->AddComponent(lampMeshRenderer);

			// Add point light component
			SpotLight* spotLight = new SpotLight(0.02f * spotLightColors[i], 0.2f * spotLightColors[i], spotLightColors[i], 2.0f, 1.2f, 0.8f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)));
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