module;
#include <glad/glad.h>
#include <glfw/glfw3.h>

module OpenGLStudy.Shader;

namespace OpenGLStudy
{
    namespace
    {
        GLuint CompileShader(GLenum type, const std::string& source)
        {
            GLuint shader = glCreateShader(type);
            const char* sourcePtr = source.c_str();
            glShaderSource(shader, 1, &sourcePtr, NULL);
            glCompileShader(shader);

            int success = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char infoLog[1024];
                glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
                std::print("Shader compilation failed:\n{}", infoLog);
                glDeleteShader(shader);
                throw std::runtime_error(std::string("Shader compilation failed:\n") + infoLog);
            }
            return shader;
        }

        std::string ReadFile(std::string_view path)
        {
            std::ifstream file;
            file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try
            {
                file.open(std::string(path));
                std::stringstream stream;
                stream << file.rdbuf();
                return stream.str();
            }
            catch (const std::ifstream::failure& e)
            {
                std::print("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: {}\n{}\n", path, e.what());
                throw std::runtime_error(std::string("Failed to read shader file: ") + std::string(path));
            }
        }
    }

    Shader::Shader(std::string_view vertexPath, std::string_view fragmentPath)
    {
        std::string vertexSource = ReadFile(vertexPath);
        std::string fragmentSource = ReadFile(fragmentPath);

        GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
        GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

        mProgram = glCreateProgram();
        glAttachShader(mProgram, vertex);
        glAttachShader(mProgram, fragment);
        glLinkProgram(mProgram);

        int success = 0;
        glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[1024];
            glGetProgramInfoLog(mProgram, sizeof(infoLog), NULL, infoLog);
            std::print("Program linking failed:\n{}", infoLog);
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            glDeleteProgram(mProgram);
            mProgram = 0;
            throw std::runtime_error(std::string("Program linking failed:\n") + infoLog);
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    Shader::~Shader()
    {
        glDeleteProgram(mProgram);
    }

    void Shader::begin() const
    {
        glUseProgram(mProgram);
    }

    void Shader::end() const
    {
        glUseProgram(0);
    }

    GLint Shader::getUniformLocation(const std::string& name) const
    {
        return glGetUniformLocation(mProgram, name.c_str());
    }

    GLint Shader::getAttribLocation(const std::string& name) const
    {
        return glGetAttribLocation(mProgram, name.c_str());
    }
}
