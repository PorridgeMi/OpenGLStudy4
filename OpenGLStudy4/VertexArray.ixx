module;
#include <glad/glad.h>
#include <glfw/glfw3.h>
export module OpenGLStudy.VertexArray;
export namespace OpenGLStudy
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		void bind() const;
		static void unbind();
		GLuint id() const { return mId; }
	private:
		GLuint mId{};
	};
}
