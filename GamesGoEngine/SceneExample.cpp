#include "SceneExample.h"

#include "Camera.h"
#include "EditorMovement.h"

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

GameObject* SceneExample::AddEditorSpectator() const
{
	GameObject* editorSpectatorObject = new GameObject();
	editorSpectatorObject->GetTransform()->SetRotationEulerDegrees(glm::vec3(0.0f, 180.0f, 0.0f));
	editorSpectatorObject->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0, 3.0f));

	EditorMovement* editorMovementComponent = new EditorMovement();
	Camera* cameraComponent = new Camera();

	editorSpectatorObject->AddComponent(editorMovementComponent);
	editorSpectatorObject->AddComponent(cameraComponent);

	std::string editorSpectatorName = "editor_spectator";
	editorSpectatorObject->SetName(editorSpectatorName);

	scene->AddGameObject(editorSpectatorObject);

	return editorSpectatorObject;
}