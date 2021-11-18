#pragma once

#include "Component.h"

class EditorMovement : public Component
{
public:
	EditorMovement();

	virtual void Update() override;

private:
	void UpdatePosition() const;
	void UpdateRotation() const;

	float movementSpeed;
	float rotationSpeed;
	bool constrainPitch;
};