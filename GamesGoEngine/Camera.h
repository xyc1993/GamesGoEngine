#pragma once

#include <glm/glm.hpp>

#include "Component.h"

class Camera : public Component
{
public:
	Camera() = default;

	void Update() override;
	
private:
	glm::mat4 GetViewMatrix() const;
};   