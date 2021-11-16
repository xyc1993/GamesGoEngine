#pragma once

#include <glm/vec3.hpp>

class Transform
{
public:
	Transform();

	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 eulerAngles);
	void SetScale(glm::vec3 scale);

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;

private:
	glm::vec3 position;
	glm::vec3 rotation; //rotation expressed in Euler Angles
	glm::vec3 scale;
};