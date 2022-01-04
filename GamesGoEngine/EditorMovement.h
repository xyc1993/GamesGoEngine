#pragma once

#include <glm/vec3.hpp>

#include "Component.h"

class EditorMovement : public Component
{
public:
	EditorMovement();

	virtual void Init(GameObject* owner) override;
	virtual void Update() override;

private:
	void UpdatePosition(const float deltaTime) const;
	void UpdateRotation(const float deltaTime);

	float movementSpeed;
	float rotationSpeed;
	bool constrainPitch;
	glm::vec3 currentRotation;
};