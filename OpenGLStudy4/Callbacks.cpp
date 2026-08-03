module;
#include<glad/glad.h>
#include<glfw/glfw3.h>
module OpenGLStudy.Callback;
namespace OpenGLStudy
{
	void OnResize(int width, int height)
	{
		std::print("OnResize: width = {}, height = {}\n", width, height);
		glViewport(0, 0, width, height);
	}

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
