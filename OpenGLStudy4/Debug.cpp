module;
#include <glad/glad.h>
#include <glfw/glfw3.h>

module OpenGLStudy.Debug;

namespace OpenGLStudy
{
    namespace
    {
        const char* SourceToString(GLenum source)
        {
            switch (source)
            {
            case GL_DEBUG_SOURCE_API:             return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:    return "WINDOW_SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER:  return "SHADER_COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY:      return "THIRD_PARTY";
            case GL_DEBUG_SOURCE_APPLICATION:      return "APPLICATION";
            default:                               return "OTHER";
            }
        }

        const char* TypeToString(GLenum type)
        {
            switch (type)
            {
            case GL_DEBUG_TYPE_ERROR:               return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:  return "DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:   return "UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY:          return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE:          return "PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER:                return "MARKER";
            default:                                 return "OTHER";
            }
        }

        const char* SeverityToString(GLenum severity)
        {
            switch (severity)
            {
            case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";
            case GL_DEBUG_SEVERITY_MEDIUM:        return "MEDIUM";
            case GL_DEBUG_SEVERITY_LOW:           return "LOW";
            default:                              return "NOTIFICATION";
            }
        }

        void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
            GLsizei length, const GLchar* message, const void* userParam)
        {
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
        GLint flags = 0;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT))
        {
            std::print("GL debug context not available, EnableGLDebugOutput skipped\n");
            return;
        }

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(DebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
}
