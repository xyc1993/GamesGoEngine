#include "Transform.h"

Transform::Transform()
{
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	UpdateModelMatrix();
	UpdateTransformDirections();
}

void Transform::SetPosition(glm::vec3 position)
{
	this->position = position;
	UpdateModelMatrix();
}

void Transform::SetRotation(glm::vec3 eulerAngles)
{
	this->rotation = eulerAngles;
	UpdateModelMatrix();
	UpdateTransformDirections();
}

void Transform::SetScale(glm::vec3 scale)
{
	this->scale = scale;
	UpdateModelMatrix();
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

glm::mat4 Transform::GetModelMatrix() const
{
	return model;
}

void Transform::UpdateTransformDirections()
{
	const glm::quat rotationQuaternion = glm::quat(this->rotation);
	forward = rotationQuaternion * glm::vec3(0.0f, 0.0f, 1.0f);
	right = rotationQuaternion * glm::vec3(-1.0f, 0.0f, 0.0f);
	up = rotationQuaternion * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Transform::UpdateModelMatrix()
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, GetPosition());
	const glm::quat rotationQuaternion = glm::quat(GetRotation());
	model = glm::rotate(model, angle(rotationQuaternion), axis(rotationQuaternion));
	model = glm::scale(model, GetScale());
}