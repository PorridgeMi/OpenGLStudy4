module;
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

module OpenGLStudy.Transform;

namespace OpenGLStudy
{
	void UploadRotationZ(int uniformLocation, float timeSeconds)
	{
		glm::mat4 transform{ 1.0f };
		transform = glm::rotate(transform, timeSeconds, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(transform));
	}
}
