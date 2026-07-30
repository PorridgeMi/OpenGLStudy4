module;
#include <glad/glad.h>
#include <glfw/glfw3.h>
export module OpenGLStudy.Application;
import std;
import OpenGLStudy.CheckError; // Application::init() 内部要调用 EnableGLDebugOutput()

export namespace OpenGLStudy
{
	// Application模块的接口文件：声明"应用程序/窗口"这个概念对外暴露的一切。
	// 具体实现（函数体）都写在Application.cpp（模块实现单元）里。
	//
	// 几种回调函数指针类型的别名。之所以用"裸函数指针"而不是std::function，
	// 是因为GLFW的C API本身要的就是函数指针，用户注册的回调也只能是无捕获的普通函数/lambda。
	using ResizeCallback = void(*)(int width, int height);
	using KeyCallback = void(*)(int key, int scancode, int action, int mods);
	using MouseButtonCallback = void(*)(int button, int action, int mods);
	using CursorPosCallback = void(*)(double xpos, double ypos);
	using ScrollCallback = void(*)(double xoffset, double yoffset);

	// GLFWwindow*不能用delete释放，必须调用glfwDestroyWindow，
	// 所以unique_ptr要显式指定"删除器"类型为一个函数指针类型（decltype(&glfwDestroyWindow)）。
	// 注意：删除器类型是函数指针时，unique_ptr没有无参默认构造函数，
	// 所以下面mWindow的默认成员初始化必须同时给出空指针和删除器两个值。
	using WindowPtr = std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>;

	// 管理一个GLFW窗口 + OpenGL上下文的单例类。
	// 单例：整个程序只应该有一个Application实例，通过getInstance()获取。
	class Application
	{
	public:
		// 给"某一类事件"注册一个用户自定义的处理函数。传nullptr表示不再关心这类事件。
		void setResizeCallback(ResizeCallback callback);
		void setKeyCallback(KeyCallback callback);
		void setMouseButtonCallback(MouseButtonCallback callback);
		void setCursorPosCallback(CursorPosCallback callback);
		void setScrollCallback(ScrollCallback callback);

		// 这几个是真正注册给GLFW的静态回调函数（GLFW的C API要求回调必须是普通函数指针，
		// 不能是非静态成员函数）。它们内部通过glfwGetWindowUserPointer拿回this指针，
		// 处理完Application自己的逻辑后，再转发给上面用户通过set*Callback注册的函数。
		static void frambufferSizeCallback(GLFWwindow* window, int width, int height);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
		static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		~Application() = default; // 没有需要手动释放的资源（mWindow是unique_ptr，自动管理），用默认析构就够
		static Application* getInstance(); // 单例访问入口，第一次调用时惰性创建实例
		// width/height/title都给了默认值，调用方可以按需只传部分参数
		bool init(int width = 800, int height = 600, const std::string& title = "OpenGL Study");
		bool update(); // 每帧调用一次：交换缓冲区+处理窗口事件；返回false表示窗口该关闭了
		void destroy(); // 释放窗口、终止GLFW；因为单例从不会被delete，析构函数不会自动跑，必须手动调用这个
		void setTitle(const std::string& title); // 运行时修改窗口标题
		int getHeight() const { return mHeight; }
		int getWidth() const { return mWidth; }
	private:
		static Application* mInstance; // 单例指针，程序退出前一直存在，故意不delete（生命周期等同整个进程）
		int mWidth{ 800 };
		int mHeight{ 600 };
		WindowPtr mWindow{ nullptr, &glfwDestroyWindow }; // 用unique_ptr自动管理窗口生命周期
		Application() = default; // 私有构造函数：外部不能直接new Application()，只能走getInstance()
		// 每种事件各自保存一个"用户注册的回调"，默认是nullptr（表示没有注册，调用前需要判空）
		ResizeCallback mResizeCallback{ nullptr };
		KeyCallback mKeyCallback{ nullptr };
		MouseButtonCallback mMouseButtonCallback{ nullptr };
		CursorPosCallback mCursorPosCallback{ nullptr };
		ScrollCallback mScrollCallback{ nullptr };

	};
}
