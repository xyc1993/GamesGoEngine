#pragma once

#include <glm/gtc/type_ptr.hpp>

#include "Component.h"

namespace GamesGoEngine
{
	class Transform : public Component
	{
	public:
		Transform();

		virtual void Update() override;
		virtual void OnSelected() override;

		void SetPosition(glm::vec3 position);
		void SetLocalPosition(glm::vec3 localPosition);
		void SetRotation(glm::quat rotation);
		void SetRotationEulerRadians(glm::vec3 eulerAngles);
		void SetRotationEulerDegrees(glm::vec3 eulerAngles);
		void SetLocalRotation(glm::quat localRotation);
		void SetLocalRotationEulerRadians(glm::vec3 eulerAngles);
		void SetLocalRotationEulerDegrees(glm::vec3 eulerAngles);
		void SetScale(glm::vec3 scale);
		void SetLocalScale(glm::vec3 localScale);
		void SetHintLocalRotation(glm::vec3 hintLocalRotation);
		void SetTransformMatrix(glm::mat4 modelMatrix);

		void UpdateTransformOnParenting();

	private:
		static void GetParentsCumulativeScale(GameObject* transformOwner, glm::vec3& cumulativeScale);

	public:
		glm::vec3 GetPosition() const;
		glm::vec3 GetLocalPosition() const;
		glm::quat GetRotation() const;
		glm::vec3 GetRotationEulerRadians() const;
		glm::vec3 GetRotationEulerDegrees() const;
		glm::quat GetLocalRotation() const;
		glm::vec3 GetLocalRotationEulerRadians() const;
		glm::vec3 GetLocalRotationEulerDegrees() const;
		glm::vec3 GetScale() const;
		glm::vec3 GetLocalScale() const;
		glm::vec3 GetHintLocalRotation() const;
		glm::vec3 GetForward() const;
		glm::vec3 GetRight() const;
		glm::vec3 GetUp() const;
		glm::mat4 GetTransformMatrix() const;

	private:
		void UpdateTransformDirections();
		void UpdateTransformMatrix();
		void UpdateChildrenTransformMatrix();

		glm::vec3 position;
		glm::vec3 localPosition;
		glm::quat rotation;
		glm::quat localRotation;
		glm::vec3 scale;
		glm::vec3 localScale;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
		glm::mat4 transformMatrix;

		// value in euler angles degrees, should only be used in properties panel UI!
		glm::vec3 hintLocalRotation;
	};
}