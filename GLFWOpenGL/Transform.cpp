#include "Transform.h"

#include <glm/gtc/type_ptr.hpp>

Transform::Transform()
{
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);

	forward = glm::vec3(0.0f, 0.0f, 1.0f);
	right = glm::vec3(1.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Transform::SetPosition(glm::vec3 position)
{
	this->position = position;
}

void Transform::SetRotation(glm::vec3 eulerAngles)
{
	this->rotation = eulerAngles;

	//update direction vectors based on the new rotation
	const glm::quat rotationQuaternion = glm::quat(this->rotation);
	forward = rotationQuaternion * glm::vec3(0.0f, 0.0f, 1.0f);
	right = rotationQuaternion * glm::vec3(-1.0f, 0.0f, 0.0f);
	up = rotationQuaternion * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Transform::SetScale(glm::vec3 scale)
{
	this->scale = scale;
}

glm::vec3 Transform::GetPosition() const
{
	return position;
}

glm::vec3 Transform::GetRotation() const
{
	return rotation;
}

glm::vec3 Transform::GetScale() const
{
	return scale;
}

glm::vec3 Transform::GetForward() const
{
	return forward;
}

glm::vec3 Transform::GetRight() const
{
	return right;
}

glm::vec3 Transform::GetUp() const
{
	return up;
}