module;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

module OpenGLStudy.Camera;

import std;

namespace OpenGLStudy
{
	Camera::Camera(glm::vec3 position)
		: mPosition(position)
	{
		updateVectors();
	}

	glm::mat4 Camera::viewMatrix() const
	{
		return glm::lookAt(mPosition, mPosition + mFront, mUp);
	}

	void Camera::processKeyboard(CameraMovement direction, float deltaTime)
	{
		float velocity = mMovementSpeed * deltaTime;
		switch (direction)
		{
		case CameraMovement::Forward:
			mPosition += mFront * velocity;
			break;
		case CameraMovement::Backward:
			mPosition -= mFront * velocity;
			break;
		case CameraMovement::Left:
			mPosition -= mRight * velocity;
			break;
		case CameraMovement::Right:
			mPosition += mRight * velocity;
			break;
		}
	}

	void Camera::processMouseMovement(float xoffset, float yoffset)
	{
		mYaw += xoffset * mMouseSensitivity;
		mPitch += yoffset * mMouseSensitivity;
		mPitch = std::clamp(mPitch, -89.0f, 89.0f);
		updateVectors();
	}

	void Camera::processMouseScroll(float yoffset)
	{
		mFov = std::clamp(mFov - yoffset, 1.0f, 45.0f);
	}

	void Camera::updateVectors()
	{
		glm::vec3 front{
			std::cos(glm::radians(mYaw)) * std::cos(glm::radians(mPitch)),
			std::sin(glm::radians(mPitch)),
			std::sin(glm::radians(mYaw)) * std::cos(glm::radians(mPitch)),
		};
		mFront = glm::normalize(front);
		mRight = glm::normalize(glm::cross(mFront, mWorldUp));
		mUp = glm::normalize(glm::cross(mRight, mFront));
	}
}
