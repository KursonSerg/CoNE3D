#include <Buffers/VertexArray.h>

CVertexArray::CVertexArray()
    : _vao(0)
{
    glGenVertexArrays(1, &_vao);
}

CVertexArray::~CVertexArray()
{
    glDeleteVertexArrays(1, &_vao);
}

void CVertexArray::bind() const
{
    glBindVertexArray(_vao);
}

void CVertexArray::unbind() const
{
    glBindVertexArray(0);
}
