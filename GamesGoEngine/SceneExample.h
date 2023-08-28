#pragma once

#include "Scene.h"

namespace GamesGoEngine
{
	class SceneExample
	{
	public:
		SceneExample();
		~SceneExample();

		virtual void Update();
		const Scene& GetScene() const;
		Scene* GetSceneUnsafe() const;

	protected:
		GameObject* AddEditorSpectator(glm::vec3 position, glm::vec3 eulerAnglesRotation) const;
		GameObject* AddEditorSpectator() const;

		Scene* scene;
	};
}