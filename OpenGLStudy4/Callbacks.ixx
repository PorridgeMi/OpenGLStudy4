module;
#include<glad/glad.h>
#include<glfw/glfw3.h>
export module OpenGLStudy.Callback;
import std;
export namespace OpenGLStudy
{
	void OnResize(int width, int height);
	void OnKey(int key, int scancode, int action, int mods);
	void OnMouseButton(int button, int action, int mods);
	void OnCursorPos(double xpos, double ypos);
	void OnScroll(double xoffset, double yoffset);
}
