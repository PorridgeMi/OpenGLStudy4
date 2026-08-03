module;
#include <glad/glad.h>
#include <glfw/glfw3.h>
export module OpenGLStudy.Shader;
import std;
export namespace OpenGLStudy
{
	class Shader
	{
	public:
		Shader(std::string_view vertexPath, std::string_view fragmentPath);
		~Shader();
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		void begin() const;
		void end() const;

		GLint getUniformLocation(const std::string& name) const;
		GLint getAttribLocation(const std::string& name) const;

		GLuint id() const { return mProgram; }
	private:
		GLuint mProgram{};
	};
}
