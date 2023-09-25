#pragma once

#include <set>
#include <string>
#include <vector>

#include "Transform.h"
#include "Component.h"

namespace GamesGoEngine
{
	class GameObject
	{
	public:
		GameObject();
		~GameObject();

		void Update();
		void LateUpdate();
		void AddComponent(Component* component);

		Transform* GetTransform() const;
		std::string GetName() const;
		void SetName(std::string name);

		GameObject* GetParent() const;
		void SetParent(GameObject* parent);

		// methods to check the status of the selection in the editor's UI
		bool IsSelected() const;
		void SetSelected(bool selected);

		int GetObjectId();

	private:
		void UpdateDirectChildrenTransforms();

	public:
		// returns number of all of the parents up the hierarchy
		int GetAllParentsNumber() const;

		void SetSceneReferenece(class Scene* scene);

		const std::vector<GameObject*>& GetChildren() const;
		const std::vector<Component*>& GetComponents() const;

	private:
		void RemoveParent();
		void SetNewParent(GameObject* parent);
		static void CalculateParentsNumber(GameObject* parent, int& currentParentNumber);
		void CleanChildren(GameObject* directChild, std::set<GameObject*> childsChildren);

		std::string name;
		Transform* transform;
		std::vector<Component*> components;

		GameObject* parent;
		// direct children of the game object
		std::vector<GameObject*> children;
		// all of the children in the game object hierarchy (i.e. children of children as well)
		std::set<GameObject*> allChildren;
		int parentsNumber;

		class Scene* scene;

		bool selected = false;
		int objectId;
		// tracks number of game objects
		static int gameObjectNumber;

	public:
		static constexpr int NAME_MAX_LENGTH = 256;
	};
}