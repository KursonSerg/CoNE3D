#include <Objects/VertexArray.h>

CVertexArray::CVertexArray()
{
    // Create a Vertex Array Object (VAO)
    glGenVertexArrays(1, &_vao);
}

CVertexArray::~CVertexArray()
{
    glDeleteVertexArrays(1, &_vao);
}

void CVertexArray::bind()
{
    // Set VAO as the current one
    glBindVertexArray(_vao);
}

void CVertexArray::unbind()
{
    glBindVertexArray(0);
}
