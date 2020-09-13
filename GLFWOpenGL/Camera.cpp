#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
{
	this->movementSpeed = SPEED;
	this->mouseSensitivity = SENSITIVITY;
	this->zoom = ZOOM;
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);

	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->UpdateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
{
	this->movementSpeed = SPEED;
	this->mouseSensitivity = SENSITIVITY;
	this->zoom = ZOOM;
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);

	this->position = glm::vec3(posX, posY, posZ);
	this->worldUp = glm::vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;
	this->UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = this->movementSpeed * deltaTime;

	if (Camera_Movement::FORWARD == direction)
	{
		this->position += this->front * velocity;
	}

	if (Camera_Movement::BACKWARD == direction)
	{
		this->position -= this->front * velocity;
	}

	if (Camera_Movement::LEFT == direction)
	{
		this->position -= this->right * velocity;
	}

	if (Camera_Movement::RIGHT == direction)
	{
		this->position += this->right * velocity;
	}

	if (Camera_Movement::UPWARD == direction)
	{
		this->position += this->up * velocity;
	}

	if (Camera_Movement::DOWNWARD == direction)
	{
		this->position -= this->up * velocity;
	}
}

void Camera::ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch)
{
	xOffset *= this->mouseSensitivity;
	yOffset *= this->mouseSensitivity;

	this->yaw += xOffset;
	this->pitch += yOffset;

	if (constrainPitch)
	{
		if (this->pitch > 89.0f)
		{
			this->pitch = 89.0f;
		}

		if (this->pitch < -89.0f)
		{
			this->pitch = -89.0f;
		}
	}

	this->UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yOffset)
{
	if (this->zoom >= 1.0f && this->zoom <= 45.0f)
	{
		this->zoom -= yOffset;
	}

	if (this->zoom <= 1.0f)
	{
		this->zoom = 1.0f;
	}

	if (this->zoom >= 45.0f)
	{
		this->zoom = 45.0f;
	}
}

GLfloat Camera::GetZoom()
{
	return this->zoom;
}

glm::vec3 Camera::GetPosition()
{
	return this->position;
}

glm::vec3 Camera::GetFront()
{
	return this->front;
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::UpdateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->up = glm::normalize(glm::cross(this->right, this->front));
}