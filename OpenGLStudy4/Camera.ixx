  module;
#include <glm/glm.hpp>
export module OpenGLStudy.Camera;

export namespace OpenGLStudy
{
	enum class CameraMovement
	{
		Forward,
		Backward,
		Left,
		Right,
	};

	class Camera
	{
	public:
		explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f));

		glm::mat4 viewMatrix() const;
		float fov() const { return mFov; }

		void processKeyboard(CameraMovement direction, float deltaTime);
		void processMouseMovement(float xoffset, float yoffset);
		void processMouseScroll(float yoffset);

	private:
		void updateVectors();

		glm::vec3 mPosition;
		glm::vec3 mFront{ 0.0f, 0.0f, -1.0f };
		glm::vec3 mUp{};
		glm::vec3 mRight{};
		glm::vec3 mWorldUp{ 0.0f, 1.0f, 0.0f };

		float mYaw{ -90.0f };
		float mPitch{ 0.0f };
		float mMovementSpeed{ 2.5f };
		float mMouseSensitivity{ 0.1f };
		float mFov{ 45.0f };
	};
}
