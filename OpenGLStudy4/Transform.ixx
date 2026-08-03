module;
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
export module OpenGLStudy.Transform;
export namespace OpenGLStudy
{
	void UploadModelRotationY(int modelLocation, float timeSeconds);
	void UploadStaticCamera(int viewLocation, int projectionLocation, float aspectRatio);
}
