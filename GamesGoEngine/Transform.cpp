#include "Transform.h"

#include "GameObject.h"

Transform::Transform()
{
	position = glm::vec3(0.0f);
	localPosition = glm::vec3(0.0f);

	rotation = glm::quat(glm::vec3(0.0f));
	localRotation = glm::quat(glm::vec3(0.0f));

	scale = glm::vec3(1.0f);
	localScale = glm::vec3(1.0f);

	UpdateModelMatrix();
	UpdateTransformDirections();
}

void Transform::Update()
{

}

void Transform::SetPosition(glm::vec3 position)
{
	const glm::vec3 translation = position - this->position;
	this->localPosition += (localScale * translation);
	this->position = position;
	UpdateModelMatrix();
}

void Transform::SetLocalPosition(glm::vec3 localPosition)
{
	const glm::vec3 translation = localPosition - this->localPosition;
	this->position += (translation / localScale);
	this->localPosition = localPosition;
	UpdateModelMatrix();
}

void Transform::SetRotation(glm::quat rotation)
{
	this->rotation = rotation;
	this->localRotation = rotation;
	UpdateModelMatrix();
	UpdateTransformDirections();
}

void Transform::SetRotationEulerRadians(glm::vec3 eulerAngles)
{
	SetRotation(glm::quat(eulerAngles));
}

void Transform::SetRotationEulerDegrees(glm::vec3 eulerAngles)
{
	SetRotationEulerRadians(glm::radians(eulerAngles));
}

void Transform::SetLocalRotation(glm::quat localRotation)
{
	this->localRotation = localRotation;
	this->rotation = localRotation;
	UpdateModelMatrix();
	UpdateTransformDirections();
}

void Transform::SetLocalRotationEulerRadians(glm::vec3 eulerAngles)
{
	SetLocalRotation(glm::quat(eulerAngles));
}

void Transform::SetLocalRotationEulerDegrees(glm::vec3 eulerAngles)
{
	SetLocalRotationEulerRadians(glm::radians(eulerAngles));
}

void Transform::SetScale(glm::vec3 scale)
{
	this->localScale *= scale / this->scale;
	this->scale = scale;
	UpdateModelMatrix();
}

void Transform::SetLocalScale(glm::vec3 localScale)
{
	this->scale *= localScale / this->localScale;
	this->localScale = localScale;
	UpdateModelMatrix();
}

void Transform::UpdateTransformOnParenting()
{
	// update local scale
	glm::vec3 cumulativeScale = glm::vec3(1.0f);
	GetParentsCumulativeScale(owner, cumulativeScale);
	localScale = scale / cumulativeScale;

	// update local position
	const glm::vec3 parentPosition = (owner != nullptr && owner->GetParent() != nullptr) ? owner->GetParent()->GetTransform()->position : glm::vec3(0.0f);
	localPosition = localScale * (position - parentPosition);

	// TODO: update local rotation
	
}

void Transform::GetParentsCumulativeScale(GameObject* transformOwner, glm::vec3& cumulativeScale)
{
	if (transformOwner != nullptr && transformOwner->GetParent() != nullptr)
	{
		cumulativeScale *= transformOwner->GetParent()->GetTransform()->GetLocalScale();
		GetParentsCumulativeScale(transformOwner->GetParent(), cumulativeScale);
	}
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
	return glm::degrees(GetRotationEulerRadians());
}

glm::quat Transform::GetLocalRotation() const
{
	return localRotation;
}

glm::vec3 Transform::GetLocalRotationEulerRadians() const
{
	return eulerAngles(GetLocalRotation());
}

glm::vec3 Transform::GetLocalRotationEulerDegrees() const
{
	return glm::degrees(GetLocalRotationEulerRadians());
}

glm::vec3 Transform::GetScale() const
{
	return scale;
}

glm::vec3 Transform::GetLocalScale() const
{
	return localScale;
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
	const glm::mat4 translate = glm::translate(glm::mat4(1.0f), GetLocalPosition());
	const glm::mat4 rotate = glm::mat4_cast(GetLocalRotation());
	const glm::mat4 scale = glm::scale(glm::mat4(1.0f), GetLocalScale());	

	model = translate * rotate * scale;

	if (owner != nullptr && owner->GetParent() != nullptr)
	{
		model = owner->GetParent()->GetTransform()->GetModelMatrix() * model;
	}

	UpdateChildrenModelMatrix();
}

void Transform::UpdateChildrenModelMatrix()
{
	if (owner != nullptr)
	{
		auto children = owner->GetChildren();
		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i] != nullptr)
			{
				children[i]->GetTransform()->UpdateModelMatrix();
			}
		}
	}
}