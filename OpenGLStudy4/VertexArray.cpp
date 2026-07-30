module;
#include <glad/glad.h>
#include <glfw/glfw3.h>

module OpenGLStudy.VertexArray;

namespace OpenGLStudy
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &mId); // 申请一个新的VAO名字（此时对象内部状态还没真正建立，要等第一次绑定）
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &mId);
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(mId);
    }

    void VertexArray::unbind()
    {
        // 绑定0表示"不绑定任何VAO"。在core profile下0不是一个合法的VAO对象，
        // 只是用来清空当前绑定状态，避免后续代码意外地改动到某个具体的VAO
        glBindVertexArray(0);
    }
}
