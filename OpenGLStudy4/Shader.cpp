module;
#include <glad/glad.h>
#include <glfw/glfw3.h>

module OpenGLStudy.Shader;

namespace OpenGLStudy
{
    // 匿名命名空间：CompileShader、ReadFile都只是内部实现细节，
    // 真正对外的入口是Shader的构造函数
    namespace
    {
        // 编译单独一个shader（顶点或片段），返回编译好的shader对象句柄。
        // 调用方负责在链接完program之后glDeleteShader释放它。
        // 编译失败时不再"打印完就当没事发生"——删掉这个没用的shader对象，然后抛异常，
        // 让上层（Shader构造函数 -> main()）有机会知道"这次没成功"，而不是带着一个无效状态继续跑。
        GLuint CompileShader(GLenum type, const std::string& source)
        {
            GLuint shader = glCreateShader(type); // 创建一个空的shader对象（还没有源码、没编译）
            const char* sourcePtr = source.c_str(); // glShaderSource要的是const char**，先转一下
            glShaderSource(shader, 1, &sourcePtr, NULL); // 把源码交给这个shader对象；NULL表示每个字符串都以\0结尾
            glCompileShader(shader); // 真正触发编译

            // 编译完不代表一定成功，必须查一下状态，失败的话打印出编译器给的详细报错信息
            int success = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char infoLog[1024];
                glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
                std::print("Shader compilation failed:\n{}", infoLog);
                glDeleteShader(shader); // 编译失败的shader对象也没用了，删掉避免泄漏
                throw std::runtime_error(std::string("Shader compilation failed:\n") + infoLog);
            }
            return shader;
        }

        // 把一个文本文件的全部内容读成一个字符串。
        // 用exceptions()开启异常，这样文件不存在/读取失败时能被catch住并打印出来，
        // 而不是像fstream默认那样"静默失败"、留一个内容不完整的字符串让人摸不着头脑。
        // 读取失败会重新抛出一个std::runtime_error——不在这里"吞掉"错误返回空字符串，
        // 因为空字符串交给CompileShader编译，报出来的错误会是"shader编译失败"，
        // 掩盖了"其实是文件根本没读到"这个真正原因，让人排查的时候摸不着头脑。
        std::string ReadFile(std::string_view path)
        {
            std::ifstream file;
            file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try
            {
                file.open(std::string(path)); // ifstream::open不接受string_view，这里要转成string
                std::stringstream stream;
                stream << file.rdbuf(); // 把文件内容整个读进字符串流
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
        // 先把两个文件分别读成字符串，再分别编译；任意一步失败都会抛异常，
        // 构造函数在这里中断执行，mProgram还是初始值0，Shader这个对象不会被"半成品"地构造出来。
        std::string vertexSource = ReadFile(vertexPath);
        std::string fragmentSource = ReadFile(fragmentPath);

        GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
        GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

        // 创建一个program对象，把两个编译好的shader"挂"上去，然后链接成一个完整的可执行程序
        mProgram = glCreateProgram();
        glAttachShader(mProgram, vertex);
        glAttachShader(mProgram, fragment);
        glLinkProgram(mProgram);

        // 链接也可能失败（比如vs的out变量和fs的in变量对不上），同样要检查状态、打印错误信息，
        // 并且把这次创建出来的GL对象都清理掉，再抛异常
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

        // 链接完成后，独立的shader对象就不再需要了（program内部已经保留了需要的信息），可以直接删除
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
        glUseProgram(0); // "停止使用任何program"是个全局操作，不需要传mProgram，直接绑定0即可
    }

    GLint Shader::getUniformLocation(const std::string& name) const
    {
        // glGetUniformLocation不要求这个program当前处于绑定状态，随时可以查
        return glGetUniformLocation(mProgram, name.c_str());
    }

    GLint Shader::getAttribLocation(const std::string& name) const
    {
        return glGetAttribLocation(mProgram, name.c_str());
    }
}
