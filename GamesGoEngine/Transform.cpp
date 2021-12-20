#include "Transform.h"

Transform::Transform()
{
	position = glm::vec3(0.0f);
	rotation = glm::quat(glm::vec3(0.0f));
	scale = glm::vec3(1.0f);
	UpdateModelMatrix();
	UpdateTransformDirections();
}

void Transform::SetPosition(glm::vec3 position)
{
	this->position = position;
	UpdateModelMatrix();
}

void Transform::SetRotation(glm::quat rotation)
{
	this->rotation = rotation;
	UpdateModelMatrix();
	UpdateTransformDirections();
}

void Transform::SetRotationEulerRadians(glm::vec3 eulerAngles)
{
	SetRotation(glm::quat(eulerAngles));
}

void Transform::SetRotationEulerDegrees(glm::vec3 eulerAngles)
{
	const glm::vec3 eulerAnglesRadians = glm::radians(eulerAngles);
	SetRotationEulerRadians(eulerAnglesRadians);
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

glm::quat Transform::GetRotation() const
{
	return rotation;
}

glm::vec3 Transform::GetRotationEulerRadians() const
{
	return eulerAngles(GetRotation());
}

glm::vec3 Transform::GetRotationEulerDegrees() const
{
	const glm::vec3 radians = GetRotationEulerRadians();
	const glm::vec3 degrees = glm::degrees(radians);
	return degrees;
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
	forward = GetRotation() * glm::vec3(0.0f, 0.0f, 1.0f);
	right = GetRotation() * glm::vec3(-1.0f, 0.0f, 0.0f);
	up = GetRotation() * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Transform::UpdateModelMatrix()
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, GetPosition());
	model = glm::rotate(model, angle(GetRotation()), axis(GetRotation()));
	model = glm::scale(model, GetScale());
}