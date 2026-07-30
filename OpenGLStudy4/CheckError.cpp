module;
#include <glad/glad.h>
#include <glfw/glfw3.h>

module OpenGLStudy.CheckError;

namespace OpenGLStudy
{
    // 匿名命名空间：里面的东西只在这个.cpp文件内可见（内部链接），
    // 相当于给一组"内部实现细节"整体加了static，外部（包括同一模块的其他文件）都看不到、用不到。
    namespace
    {
        // 把GLenum形式的错误来源转换成人能看懂的字符串，纯粹用于打印日志
        const char* SourceToString(GLenum source)
        {
            switch (source)
            {
            case GL_DEBUG_SOURCE_API:             return "API";              // 调用OpenGL API本身触发的
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:    return "WINDOW_SYSTEM";    // 窗口系统(比如GLFW/WGL)触发的
            case GL_DEBUG_SOURCE_SHADER_COMPILER:  return "SHADER_COMPILER";  // shader编译器触发的
            case GL_DEBUG_SOURCE_THIRD_PARTY:      return "THIRD_PARTY";      // 第三方库触发的
            case GL_DEBUG_SOURCE_APPLICATION:      return "APPLICATION";      // 应用程序自己主动上报的
            default:                               return "OTHER";
            }
        }

        // 把错误类型转成字符串
        const char* TypeToString(GLenum type)
        {
            switch (type)
            {
            case GL_DEBUG_TYPE_ERROR:               return "ERROR";               // 真正的错误
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:  return "DEPRECATED_BEHAVIOR"; // 用了废弃的用法
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:   return "UNDEFINED_BEHAVIOR";  // 触发了未定义行为
            case GL_DEBUG_TYPE_PORTABILITY:          return "PORTABILITY";        // 可能在别的平台/驱动上不能用
            case GL_DEBUG_TYPE_PERFORMANCE:          return "PERFORMANCE";        // 性能警告
            case GL_DEBUG_TYPE_MARKER:                return "MARKER";             // 调试标记，非错误
            default:                                 return "OTHER";
            }
        }

        // 把严重程度转成字符串
        const char* SeverityToString(GLenum severity)
        {
            switch (severity)
            {
            case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";           // 严重错误
            case GL_DEBUG_SEVERITY_MEDIUM:        return "MEDIUM";         // 中等
            case GL_DEBUG_SEVERITY_LOW:           return "LOW";            // 轻微
            default:                              return "NOTIFICATION";   // 只是提示，不是错误
            }
        }

        // 真正的回调函数：驱动检测到问题时会调用这个函数。
        // 签名必须严格匹配GLDEBUGPROC这个类型（包括APIENTRY调用约定），否则会崩溃或行为异常。
        void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
            GLsizei length, const GLchar* message, const void* userParam)
        {
            // NOTIFICATION级别的太多、太琐碎（一般是驱动内部的一些提示），过滤掉，只关心真正的问题
            if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
            {
                return;
            }
            std::println("[OpenGL Debug][{}][{}][{}] ({}): {}",
                SourceToString(source), TypeToString(type), SeverityToString(severity), id, message);
        }
    }

    void EnableGLDebugOutput()
    {
        // 先查一下当前上下文是不是真的带了"调试上下文"标志位
        // （必须在创建窗口时用glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE)请求过，驱动才会给）
        GLint flags = 0;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT))
        {
            std::print("GL debug context not available, EnableGLDebugOutput skipped\n");
            return; // 没有调试上下文就直接放弃，不然下面的调用可能没有实际效果
        }

        glEnable(GL_DEBUG_OUTPUT);            // 开启调试输出总开关
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // 同步模式：错误会在触发它的那次GL调用内部立刻报告，
                                               // 方便在调试器里下断点时，调用栈就是出错的那一行代码
        glDebugMessageCallback(DebugCallback, nullptr); // 注册回调函数，nullptr是传给回调的userParam(用不到)
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE); // 不过滤，全部类型都上报
    }
}
