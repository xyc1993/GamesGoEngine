#include "SceneExample_DepthTest.h"

#include "Camera.h"
#include "EditorMovement.h"
#include "MeshImported.h"
#include "MeshPrimitiveCube.h"
#include "MeshRenderer.h"
#include "PositionOscillator.h"
#include "Rotator.h"
#include "ScaleOscillator.h"

namespace GamesGoEngine
{
	SceneExample_DepthTest::SceneExample_DepthTest()
	{
		std::shared_ptr<Material> depthMaterial = std::make_shared<Material>("res/shaders/unlit.vert.glsl", "res/shaders/depth.frag.glsl");

		const int BOXES_NUMBER = 10;

		glm::vec3 boxesPositions[BOXES_NUMBER] = {
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

		glm::vec3 boxesRotations[BOXES_NUMBER] = {
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

		std::shared_ptr<MeshPrimitiveCube> cubeMesh = std::make_shared<MeshPrimitiveCube>();

		GameObject* boxChild1 = nullptr;
		GameObject* boxChild2 = nullptr;
		GameObject* boxChild3 = nullptr;
		for (int i = 0; i < BOXES_NUMBER; i++)
		{
			GameObject* boxObject = new GameObject();
			MeshRenderer* litBoxesMeshRenderer = new MeshRenderer();
			litBoxesMeshRenderer->SetMesh(cubeMesh);
			litBoxesMeshRenderer->SetMaterial(depthMaterial);
			boxObject->AddComponent(litBoxesMeshRenderer);
			boxObject->GetTransform()->SetPosition(boxesPositions[i]);
			boxObject->GetTransform()->SetRotationEulerDegrees(boxesRotations[i]);

			std::string name = "box_";
			name.append(std::to_string(i));
			boxObject->SetName(name);

			scene->AddGameObject(boxObject);
			if (i == 0)
			{
				boxChild1 = boxObject;
			}
			if (i == 1)
			{
				boxChild2 = boxObject;
			}
			if (i == 3)
			{
				boxChild3 = boxObject;
			}
		}

		if (boxChild1 != nullptr && boxChild2 != nullptr && boxChild3 != nullptr)
		{
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

		AddEditorSpectator();
	}
}