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
	glm::vec3 GetForward() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;

private:
	glm::vec3 position;
	glm::vec3 rotation; //rotation expressed in Euler Angles
	glm::vec3 scale;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
};