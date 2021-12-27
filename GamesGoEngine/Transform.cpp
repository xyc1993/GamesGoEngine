#include "Transform.h"

#include "GameObject.h"

Transform::Transform()
{
	position = glm::vec3(0.0f);
	rotation = glm::quat(glm::vec3(0.0f));
	scale = glm::vec3(1.0f);
	UpdateModelMatrix();
	UpdateTransformDirections();
}

void Transform::Update()
{

}

void Transform::SetPosition(glm::vec3 position)
{
	if (owner == nullptr)
	{
		return;
	}

	const glm::vec3 translation = position - this->position;

	// update self
	this->position = position;
	if (owner->GetParent() != nullptr)
	{
		this->localPosition = this->position - owner->GetParent()->GetTransform()->GetPosition();
	}
	else
	{
		this->localPosition = this->position;
	}
	UpdateModelMatrix();

	// update children
	for (int i = 0; i < owner->GetChildren().size(); i++)
	{
		GameObject* child = owner->GetChildren()[i];
		if (child != nullptr)
		{
			const glm::vec3 currentPosition = child->GetTransform()->GetPosition();
			child->GetTransform()->SetPosition(currentPosition + translation);
		}
	}
}

void Transform::SetLocalPosition(glm::vec3 localPosition)
{
	const glm::vec3 translation = localPosition - this->localPosition;
	SetPosition(this->position + translation);
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

glm::vec3 Transform::GetLocalPosition() const
{
	return localPosition;
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
	const glm::mat4 translate = glm::translate(glm::mat4(1.0f), GetPosition());
	const glm::mat4 rotate = glm::mat4_cast(GetRotation());
	const glm::mat4 scale = glm::scale(glm::mat4(1.0f), GetScale());

	model = translate * rotate * scale;
}