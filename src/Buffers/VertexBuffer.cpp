#include <Buffers/VertexBuffer.h>

CVertexBuffer::CVertexBuffer()
    : _vbo(0)
{
    glGenBuffers(1, &_vbo);
}

CVertexBuffer::~CVertexBuffer()
{
    glDeleteBuffers(1, &_vbo);
}

void CVertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}

void CVertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CVertexBuffer::updateData(GLsizeiptr size, const void *data, GLenum usage)
{
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void CVertexBuffer::updateAttrib(GLuint index, GLint size, GLsizei stride, GLuint offset)
{
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(offset));
    glEnableVertexAttribArray(index);
}
