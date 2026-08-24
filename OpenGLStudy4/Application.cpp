module;
#include <glad/glad.h>
#include <glfw/glfw3.h>

module OpenGLStudy.Application;

import OpenGLStudy.Debug;

namespace OpenGLStudy
{
    Application* Application::mInstance = nullptr;

    Application* Application::getInstance()
    {
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
        mTitle = title;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        mWindow.reset(glfwCreateWindow(mWidth, mHeight, title.c_str(), NULL, NULL));
        if (mWindow == nullptr)
        {
            std::print("Failed to create GLFW window\n");
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(mWindow.get());

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::print("Failed to initialize GLAD\n");
            mWindow.reset();
            glfwTerminate();
            return false;
        }

#ifdef _DEBUG
        constexpr bool kIsDebugBuild = true;
#else
        constexpr bool kIsDebugBuild = false;
#endif
        if constexpr (kIsDebugBuild)
        {
            EnableGLDebugOutput();
        }

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
        if (glfwWindowShouldClose(mWindow.get()))
        {
            return false;
        }
        glfwSwapBuffers(mWindow.get());
        glfwPollEvents();
        return true;
    }

    void Application::setTitle(const std::string& title)
    {
        mTitle = title;
        glfwSetWindowTitle(mWindow.get(), title.c_str());
    }

    void Application::setCursorCaptured(bool captured)
    {
        glfwSetInputMode(mWindow.get(), GLFW_CURSOR, captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    bool Application::isKeyPressed(int key) const
    {
        return glfwGetKey(mWindow.get(), key) == GLFW_PRESS;
    }

    void Application::destroy()
    {
        mWindow.reset();
        glfwTerminate();
    }

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

    void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app->mMouseButtonCallback)
        {
            app->mMouseButtonCallback(button, action, mods);
        }
    }

    void Application::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app->mCursorPosCallback)
        {
            app->mCursorPosCallback(xpos, ypos);
        }
    }

    void Application::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app->mScrollCallback)
        {
            app->mScrollCallback(xoffset, yoffset);
        }
    }

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
