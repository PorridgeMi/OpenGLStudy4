module;
#include <glad/glad.h>

module OpenGLStudy.Texture;

import OpenGLStudy.Image;

namespace OpenGLStudy
{
	Texture::Texture(std::string_view path, GLuint unit)
		: mUnit(unit)
	{
		Image image(std::string(path).c_str());
		if (!image.isValid())
		{
			std::print("Failed to load texture: {}\n{}\n", path, image.failureReason());
			throw std::runtime_error(std::string("Failed to load texture: ") + std::string(path));
		}

		glActiveTexture(GL_TEXTURE0 + mUnit);
		glGenTextures(1, &mId);
		glBindTexture(GL_TEXTURE_2D, mId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum format = image.channels() == 4 ? GL_RGBA : GL_RGB;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, format, image.width(), image.height(), 0, format, GL_UNSIGNED_BYTE, image.data());
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &mId);
	}

	void Texture::bind() const
	{
		glActiveTexture(GL_TEXTURE0 + mUnit);
		glBindTexture(GL_TEXTURE_2D, mId);
	}

	void Texture::unbind() const
	{
		glActiveTexture(GL_TEXTURE0 + mUnit);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
