#include <Objects/IndexBuffer.h>

CIndexBuffer::CIndexBuffer()
{
    glGenBuffers(1, &_ibo);
}

CIndexBuffer::~CIndexBuffer()
{
    glDeleteBuffers(1, &_ibo);
}

void CIndexBuffer::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
}

void CIndexBuffer::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CIndexBuffer::updateData(GLsizeiptr size, const void *data, GLenum usage)
{
    // Fill IBO with indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}
