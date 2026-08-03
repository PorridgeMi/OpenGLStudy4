module;
#include <glad/glad.h>
#include <glfw/glfw3.h>

module OpenGLStudy.VertexArray;

namespace OpenGLStudy
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &mId);
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
        glBindVertexArray(0);
    }
}
