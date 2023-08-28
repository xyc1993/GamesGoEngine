#include "SceneExample.h"

#include "Camera.h"
#include "EditorMovement.h"

namespace GamesGoEngine
{
	SceneExample::SceneExample()
	{
		scene = new Scene();
	}

	SceneExample::~SceneExample()
	{
		delete scene;
	}

	void SceneExample::Update()
	{
		scene->Update();
	}

	const Scene& SceneExample::GetScene() const
	{
		return *scene;
	}

	Scene* SceneExample::GetSceneUnsafe() const
	{
		return scene;
	}

	GameObject* SceneExample::AddEditorSpectator(glm::vec3 position, glm::vec3 eulerAnglesRotation) const
	{
		GameObject* editorSpectatorObject = new GameObject();
		editorSpectatorObject->GetTransform()->SetRotationEulerDegrees(eulerAnglesRotation);
		editorSpectatorObject->GetTransform()->SetPosition(position);

		EditorMovement* editorMovementComponent = new EditorMovement();
		Camera* cameraComponent = new Camera();

		editorSpectatorObject->AddComponent(editorMovementComponent);
		editorSpectatorObject->AddComponent(cameraComponent);

		std::string editorSpectatorName = "editor_spectator";
		editorSpectatorObject->SetName(editorSpectatorName);

		scene->AddGameObject(editorSpectatorObject);

		return editorSpectatorObject;
	}

	GameObject* SceneExample::AddEditorSpectator() const
	{
		return AddEditorSpectator(glm::vec3(0.0f, 0.0, 3.0f), glm::vec3(0.0f, 180.0f, 0.0f));
	}
}