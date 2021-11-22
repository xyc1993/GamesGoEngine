#pragma once

#include <glm/gtc/type_ptr.hpp>

class Transform
{
public:
	Transform();

	void SetPosition(glm::vec3 position);
	void SetRotation(glm::quat rotation);
	void SetRotationEulerRadians(glm::vec3 eulerAngles);
	void SetRotationEulerDegrees(glm::vec3 eulerAngles);
	void SetScale(glm::vec3 scale);

	glm::vec3 GetPosition() const;
	glm::quat GetRotation() const;
	glm::vec3 GetRotationEulerRadians() const;
	glm::vec3 GetRotationEulerDegrees() const;
	glm::vec3 GetScale() const;
	glm::vec3 GetForward() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;
	glm::mat4 GetModelMatrix() const;

private:
	void UpdateTransformDirections();
	void UpdateModelMatrix();

	glm::vec3 position;	
	glm::quat rotation;
	glm::vec3 scale;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::mat4 model;
};