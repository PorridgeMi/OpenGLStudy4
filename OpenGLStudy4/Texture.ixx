module;
#include <glad/glad.h>
export module OpenGLStudy.Texture;
import std;
export namespace OpenGLStudy
{
	class Texture
	{
	public:
		Texture(std::string_view path, GLuint unit);
		~Texture();
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		void bind() const;
		void unbind() const;
		GLuint id() const { return mId; }
		GLuint unit() const { return mUnit; }
	private:
		GLuint mId{};
		GLuint mUnit{};
	};
}
