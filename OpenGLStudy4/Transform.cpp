module;
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

module OpenGLStudy.Transform;

namespace OpenGLStudy
{
	void UploadModelRotation(int modelLocation, float timeSeconds)
	{
		glm::mat4 model{ 1.0f };
		model = glm::rotate(model, timeSeconds * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	}

	void UploadView(int viewLocation, const glm::mat4& view)
	{
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	}

	void UploadPerspectiveProjection(int projectionLocation, float fovDegrees, float aspectRatio)
	{
		glm::mat4 projection = glm::perspective(glm::radians(fovDegrees), aspectRatio, 0.1f, 100.0f);
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
	}
}
