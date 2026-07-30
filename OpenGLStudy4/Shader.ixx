module;
#include <glad/glad.h>
#include <glfw/glfw3.h>
export module OpenGLStudy.Shader;
import std;
export namespace OpenGLStudy
{
	// 给定顶点/片段shader的文件路径，负责读文件、编译、链接成一个可用的program，
	// 并用RAII管理它的生命周期（析构时自动glDeleteProgram）。
	class Shader
	{
	public:
		Shader(std::string_view vertexPath, std::string_view fragmentPath);
		~Shader();
		// 禁止拷贝：如果允许拷贝，两个Shader对象会指向同一个GL程序对象(mProgram相同)，
		// 其中一个析构时删掉了，另一个的mProgram就变成悬空的了，所以直接禁掉拷贝更安全
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		void begin() const; // 开始使用这个shader program
		void end() const;   // 停止使用（绑定0）

		// 查询某个uniform/顶点属性变量在这个program里的"位置"(location)。
		// 如果传入的名字在shader里不存在（或者没被用到、被编译器优化掉了），会返回-1。
		GLint getUniformLocation(const std::string& name) const;
		GLint getAttribLocation(const std::string& name) const;

		GLuint id() const { return mProgram; } // 暴露底层GL句柄，供需要直接调GL API的场景使用
	private:
		GLuint mProgram{}; // 底层OpenGL程序对象的句柄
	};
}
