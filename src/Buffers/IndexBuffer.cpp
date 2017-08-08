#include <Buffers/IndexBuffer.h>

CIndexBuffer::CIndexBuffer()
    : _ibo(0)
{
    glGenBuffers(1, &_ibo);
}

CIndexBuffer::~CIndexBuffer()
{
    glDeleteBuffers(1, &_ibo);
}

void CIndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
}

void CIndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CIndexBuffer::updateData(GLsizeiptr size, const void *data, GLenum usage)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}
