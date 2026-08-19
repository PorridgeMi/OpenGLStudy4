module;
#include <glad/glad.h>
export module OpenGLStudy.Texture;
import std;
export namespace OpenGLStudy
{
	class Texture
	{
	public:
		explicit Texture(std::string_view path);
		~Texture();
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		void bind(GLuint unit = 0) const;
		static void unbind(GLuint unit = 0);
		GLuint id() const { return mId; }
	private:
		GLuint mId{};
	};
}
