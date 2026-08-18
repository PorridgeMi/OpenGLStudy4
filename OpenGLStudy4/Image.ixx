export module OpenGLStudy.Image;
export namespace OpenGLStudy
{
	class Image
	{
	public:
		explicit Image(const char* path);
		~Image();
		Image(const Image&) = delete;
		Image& operator=(const Image&) = delete;

		bool isValid() const { return mData != nullptr; }
		const unsigned char* data() const { return mData; }
		int width() const { return mWidth; }
		int height() const { return mHeight; }
		int channels() const { return mChannels; }
		const char* failureReason() const;
	private:
		unsigned char* mData{ nullptr };
		int mWidth{};
		int mHeight{};
		int mChannels{};
	};
}
