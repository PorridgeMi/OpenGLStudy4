module;
#include <glad/glad.h>
#include <glfw/glfw3.h>

module OpenGLStudy.Application;

import OpenGLStudy.CheckError;

namespace OpenGLStudy
{
    // Application模块的实现单元：这里是Application.ixx里声明的每个函数的具体实现。
    // 静态成员必须在类外定义一次，程序启动时初始化为nullptr（表示还没创建过实例）
    Application* Application::mInstance = nullptr;

    Application* Application::getInstance()
    {
        // 惰性初始化：第一次调用时才真正创建实例，之后一直复用同一个
        if (mInstance == nullptr)
        {
            mInstance = new Application();
        }
        return mInstance;
    }

    bool Application::init(int width, int height, const std::string& title)
    {
        mWidth = width;
        mHeight = height;

        glfwInit(); // 初始化GLFW库本身，必须在任何其他GLFW调用之前执行
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // 请求OpenGL 4.6
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 用core profile（没有旧版兼容API，比如没有默认VAO）
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE); // 请求一个"调试上下文"，配合EnableGLDebugOutput使用

        // glfwCreateWindow返回一个裸的GLFWwindow*，reset()会把它交给mWindow这个unique_ptr托管，
        // 之后mWindow析构时会自动调用glfwDestroyWindow释放
        mWindow.reset(glfwCreateWindow(mWidth, mHeight, title.c_str(), NULL, NULL));
        if (mWindow == nullptr)
        {
            std::print("Failed to create GLFW window\n");
            glfwTerminate(); // 窗口没建成功，但glfwInit已经跑过了，要对应调用一次glfwTerminate收尾
            return false;
        }

        glfwMakeContextCurrent(mWindow.get()); // 把这个窗口的OpenGL上下文设为"当前线程正在使用的上下文"

        // GLAD负责在运行时查询、加载各个OpenGL函数的真实地址（不同显卡驱动提供的地址不一样）
        // 必须在有了当前上下文之后才能加载，加载完之后才能调用任何gl开头的函数
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::print("Failed to initialize GLAD\n");
            mWindow.reset(); // 显式释放窗口（不等到析构，因为init失败后调用方可能不会再调用destroy）
            glfwTerminate();
            return false;
        }

        EnableGLDebugOutput(); // 注册GL调试回调，后续OpenGL调用出错时会自动打印详细信息

        // 把this指针挂到窗口的"用户指针"槽位上，这样在下面几个静态回调里，
        // 可以通过glfwGetWindowUserPointer(window)把它取回来，从而访问到具体的Application实例。
        // 必须在注册下面这几个回调之前调用，否则回调触发时用户指针可能还没设置。
        glfwSetWindowUserPointer(mWindow.get(), this);
        glfwSetFramebufferSizeCallback(mWindow.get(), frambufferSizeCallback);
        glfwSetKeyCallback(mWindow.get(), keyCallback);
        glfwSetMouseButtonCallback(mWindow.get(), mouseButtonCallback);
        glfwSetCursorPosCallback(mWindow.get(), cursorPosCallback);
        glfwSetScrollCallback(mWindow.get(), scrollCallback);

        return true;
    }

    bool Application::update()
    {
        // glfwWindowShouldClose在用户点击关闭按钮、或者keyCallback里主动设置了"应该关闭"标志后返回true
        if (glfwWindowShouldClose(mWindow.get()))
        {
            return false;
        }
        glfwSwapBuffers(mWindow.get()); // 双缓冲：把这一帧画好的后台缓冲区交换到前台显示
        glfwPollEvents();               // 处理累积的窗口消息（键盘/鼠标/窗口大小变化等），会触发上面注册的回调
        return true;
    }

    void Application::setTitle(const std::string& title)
    {
        glfwSetWindowTitle(mWindow.get(), title.c_str());
    }

    void Application::destroy()
    {
        mWindow.reset();  // 显式释放窗口（unique_ptr::reset会调用glfwDestroyWindow）
        glfwTerminate();  // 释放GLFW库自身占用的所有资源
    }

    // 窗口大小变化时被GLFW调用。先做Application自己必须做的事（更新viewport），
    // 再看用户有没有注册自己的处理函数，有的话转发过去。
    void Application::frambufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        app->mWidth = width;
        app->mHeight = height;
        glViewport(0, 0, width, height);
        if (app->mResizeCallback)
        {
            app->mResizeCallback(width, height);
        }
    }

    // 按键事件回调。内置了"按ESC关闭窗口"这个固定行为，之后再转发给用户注册的回调（如果有的话）。
    void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app->mKeyCallback)
        {
            app->mKeyCallback(key, scancode, action, mods);
        }
    }

    // 鼠标按键事件回调，没有内置行为，纯转发给用户注册的回调
    void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app->mMouseButtonCallback)
        {
            app->mMouseButtonCallback(button, action, mods);
        }
    }

    // 鼠标移动事件回调，纯转发
    void Application::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app->mCursorPosCallback)
        {
            app->mCursorPosCallback(xpos, ypos);
        }
    }

    // 滚轮事件回调，纯转发
    void Application::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app->mScrollCallback)
        {
            app->mScrollCallback(xoffset, yoffset);
        }
    }

    // 下面这几个set*Callback都是很朴素的"存一下用户传进来的std::function"，
    // 用std::move避免多拷贝一次（std::function内部可能持有堆分配的捕获状态）；
    // 真正被调用是在上面那几个静态回调里
    void Application::setResizeCallback(ResizeCallback callback)
    {
        mResizeCallback = std::move(callback);
    }

    void Application::setKeyCallback(KeyCallback callback)
    {
        mKeyCallback = std::move(callback);
    }

    void Application::setMouseButtonCallback(MouseButtonCallback callback)
    {
        mMouseButtonCallback = std::move(callback);
    }

    void Application::setCursorPosCallback(CursorPosCallback callback)
    {
        mCursorPosCallback = std::move(callback);
    }

    void Application::setScrollCallback(ScrollCallback callback)
    {
        mScrollCallback = std::move(callback);
    }
}
