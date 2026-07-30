module;
#include <glad/glad.h>
#include <glfw/glfw3.h>
export module OpenGLStudy.CheckError;
import std;
export namespace OpenGLStudy
{
	// 开启OpenGL的调试输出机制（GL 4.3+/KHR_debug特性）。
	// 比手动到处调用glGetError()轮询错误更省心：一旦开启，驱动有错误/警告会自动回调过来。
	void EnableGLDebugOutput();
}
