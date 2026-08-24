module;
#include <glad/glad.h>
#include <glfw/glfw3.h>
export module OpenGLStudy.Application;
import std;
import OpenGLStudy.Debug;

export namespace OpenGLStudy
{
	using ResizeCallback = std::function<void(int width, int height)>;
	using KeyCallback = std::function<void(int key, int scancode, int action, int mods)>;
	using MouseButtonCallback = std::function<void(int button, int action, int mods)>;
	using CursorPosCallback = std::function<void(double xpos, double ypos)>;
	using ScrollCallback = std::function<void(double xoffset, double yoffset)>;

	using WindowPtr = std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>;

	class Application
	{
	public:
		void setResizeCallback(ResizeCallback callback);
		void setKeyCallback(KeyCallback callback);
		void setMouseButtonCallback(MouseButtonCallback callback);
		void setCursorPosCallback(CursorPosCallback callback);
		void setScrollCallback(ScrollCallback callback);

		static void frambufferSizeCallback(GLFWwindow* window, int width, int height);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
		static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		~Application() = default;
		static Application* getInstance();
		bool init(int width = 800, int height = 600, const std::string& title = "OpenGL Study");
		bool update();
		void destroy();
		void setTitle(const std::string& title);
		void setCursorCaptured(bool captured);
		bool isKeyPressed(int key) const;
		int getHeight() const { return mHeight; }
		int getWidth() const { return mWidth; }
		const std::string& getTitle() const { return mTitle; }
	private:
		static Application* mInstance;
		int mWidth{ 800 };
		int mHeight{ 600 };
		std::string mTitle{ "OpenGL Study" };
		WindowPtr mWindow{ nullptr, &glfwDestroyWindow };
		Application() = default;
		ResizeCallback mResizeCallback{};
		KeyCallback mKeyCallback{};
		MouseButtonCallback mMouseButtonCallback{};
		CursorPosCallback mCursorPosCallback{};
		ScrollCallback mScrollCallback{};

	};
}
