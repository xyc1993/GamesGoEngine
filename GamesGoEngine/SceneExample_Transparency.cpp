#include "SceneExample_Transparency.h"

#include <memory>

#include "Material.h"
#include "MeshPrimitiveQuad.h"
#include "MeshRenderer.h"

namespace GamesGoEngine
{
	SceneExample_Transparency::SceneExample_Transparency()
	{
		std::vector<glm::vec3> grassPositions;
		grassPositions.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
		grassPositions.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
		grassPositions.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
		grassPositions.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
		grassPositions.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

		std::shared_ptr<MeshPrimitiveQuad> quadMesh = std::make_shared<MeshPrimitiveQuad>();

		std::shared_ptr<Material> grassMaterial = std::make_shared<Material>("res/shaders/unlitTextured.vert.glsl", "res/shaders/unlitTexturedMasked.frag.glsl");
		grassMaterial->SetTextureByPath("mainTexture", 0, (GLchar*)"res/textures/grass.png", true);

		for (size_t i = 0; i < grassPositions.size(); i++)
		{
			GameObject* grassObject = new GameObject();
			MeshRenderer* meshRenderer = new MeshRenderer();
			meshRenderer->SetMesh(quadMesh);
			meshRenderer->SetMaterial(grassMaterial);
			grassObject->AddComponent(meshRenderer);
			grassObject->GetTransform()->SetPosition(grassPositions[i]);

			std::string name = "grass_";
			name.append(std::to_string(i));
			grassObject->SetName(name);

			scene->AddGameObject(grassObject);
		}

		std::shared_ptr<Material> windowMaterial = std::make_shared<Material>("res/shaders/unlitTextured.vert.glsl", "res/shaders/unlitTexturedTransparent.frag.glsl");
		windowMaterial->SetTextureByPath("mainTexture", 0, (GLchar*)"res/textures/blending_transparent_window.png", true);

		for (size_t i = 0; i < grassPositions.size(); i++)
		{
			GameObject* windowObject = new GameObject();
			MeshRenderer* meshRenderer = new MeshRenderer();
			meshRenderer->SetMesh(quadMesh);
			meshRenderer->SetMaterial(windowMaterial);
			meshRenderer->SetRenderQueuePosition(RenderQueuePosition::Transparent);
			windowObject->AddComponent(meshRenderer);
			windowObject->GetTransform()->SetPosition(grassPositions[i] + glm::vec3(0.0f, 0.0f, 1.0f));

			std::string name = "window_";
			name.append(std::to_string(i));
			windowObject->SetName(name);

			scene->AddGameObject(windowObject);
		}

		AddEditorSpectator();
	}
}