module;
#include<glad/glad.h>
#include<glfw/glfw3.h>
export module OpenGLStudy.Callback;
import std;
export namespace OpenGLStudy
{
	// 窗口事件的示例处理函数：目前每个都只是打印一下收到的参数，
	// 用setXxxCallback注册给Application之后，对应事件发生时就会被调用。
	// 模块名叫OpenGLStudy.Callback（单数），文件名叫Callbacks（复数），两者不需要一致，
	// 模块名是import的时候用的名字，跟文件名是两回事。
	void OnResize(int width, int height);       // 窗口大小变化时调用
	void OnKey(int key, int scancode, int action, int mods); // 键盘按键时调用
	void OnMouseButton(int button, int action, int mods);    // 鼠标按键时调用
	void OnCursorPos(double xpos, double ypos);  // 鼠标移动时调用
	void OnScroll(double xoffset, double yoffset); // 滚轮滚动时调用
}
