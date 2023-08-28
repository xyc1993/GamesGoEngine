#pragma once

#include <glm/glm.hpp>

#include "Component.h"

namespace GamesGoEngine
{
	class Camera : public Component
	{
	public:
		Camera();
		~Camera();

		virtual void Init(GameObject* owner) override;
		virtual void Update() override;

	private:
		void CalculateViewMatrix();
		void CalculateProjectionMatrix();

	public:
		void SetCameraPriority(int priority);
		void SetFoV(float fov);
		void SetNearClipPlane(float nearClipPlane);
		void SetFarClipPlane(float farClipPlane);

		glm::mat4 GetCurrentViewMatrix() const;
		glm::mat4 GetCurrentProjectionMatrix() const;
		int GetCameraPriority() const;
		float GetFoV() const;
		float GetNearClipPlane() const;
		float GetFarClipPlane() const;

	private:
		glm::mat4 currentViewMatrix;
		glm::mat4 currentProjectionMatrix;

		int priority;
		float fov;
		float nearClipPlane;
		float farClipPlane;
	};
}