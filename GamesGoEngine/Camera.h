#pragma once

#include <glm/glm.hpp>

#include "Component.h"

class Camera : public Component
{
public:
	Camera();
	~Camera();

	virtual void Init(GameObject* owner) override;
	virtual void Update() override;

	void SetCameraPriority(int priority);

	glm::mat4 GetCurrentViewMatrix() const;
	int GetCameraPriority() const;

	
private:
	glm::mat4 GetViewMatrix() const;

	glm::mat4 currentViewMatrix;
	int priority;
};   