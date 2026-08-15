module;
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

module OpenGLStudy.Image;

namespace OpenGLStudy
{
	Image::Image(const char* path)
	{
		stbi_set_flip_vertically_on_load(true);
		mData = stbi_load(path, &mWidth, &mHeight, &mChannels, 0);
	}

	Image::~Image()
	{
		stbi_image_free(mData);
	}

	const char* Image::failureReason() const
	{
		return stbi_failure_reason();
	}
}
