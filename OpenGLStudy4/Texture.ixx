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

		void bind() const;
		static void unbind();
		GLuint id() const { return mId; }
	private:
		GLuint mId{};
	};
}
