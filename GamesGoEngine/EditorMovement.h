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
	void UpdatePosition() const;
	void UpdateRotation();

	float movementSpeed;
	float rotationSpeed;
	bool constrainPitch;
	glm::vec3 currentRotation;
};