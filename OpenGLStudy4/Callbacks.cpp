module;
#include<glad/glad.h>
#include<glfw/glfw3.h>
module OpenGLStudy.Callback;
// 注意：这个文件没写import std;——因为它是OpenGLStudy.Callback模块自己的实现单元，
// 跟接口文件Callbacks.ixx属于同一个模块，会自动共享接口文件里import过的东西（包括std），
// 不用重复写。（写不写都能编译过，重复写是为了可读性，这里省略是为了演示这条规则）
namespace OpenGLStudy
{
	// 窗口大小变化时触发：同时更新一下viewport，保证渲染结果始终填满整个窗口
	void OnResize(int width, int height)
	{
		std::print("OnResize: width = {}, height = {}\n", width, height);
		glViewport(0, 0, width, height);
	}

	// 按键事件：这里只是打印，真正"按ESC关闭窗口"的逻辑写在Application::keyCallback里，
	// 跟这个函数无关（这个函数只是转发链路的最后一环，用户自定义的额外逻辑）
	void OnKey(int key, int scancode, int action, int mods)
	{
		std::print("OnKey: key = {}, action = {}\n", key, action);
	}

	void OnMouseButton(int button, int action, int mods)
	{
		std::print("OnMouseButton: button = {}, action = {}\n", button, action);
	}

	void OnCursorPos(double xpos, double ypos)
	{
		std::print("OnCursorPos: x = {}, y = {}\n", xpos, ypos);
	}

	void OnScroll(double xoffset, double yoffset)
	{
		std::print("OnScroll: xoffset = {}, yoffset = {}\n", xoffset, yoffset);
	}



}
