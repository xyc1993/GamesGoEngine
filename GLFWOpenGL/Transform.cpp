#include "Transform.h"

Transform::Transform()
{
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
}

void Transform::SetPosition(glm::vec3 position)
{
	this->position = position;
}

void Transform::SetRotation(glm::vec3 eulerAngles)
{
	this->rotation = eulerAngles;
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
