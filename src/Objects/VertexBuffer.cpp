#include <Objects/VertexBuffer.h>

CVertexBuffer::CVertexBuffer()
    : _vbo(0)
{
    // Create a Vertex Buffer Object (VBO)
    glGenBuffers(1, &_vbo);
}

CVertexBuffer::~CVertexBuffer()
{
    glDeleteBuffers(1, &_vbo);
}

void CVertexBuffer::bind()
{
    // Set VBO as the current one
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}

void CVertexBuffer::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CVertexBuffer::updateData(GLsizeiptr size, const void *data, GLenum usage)
{
    // Fill VBO with vertices
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void CVertexBuffer::updateAttrib(GLuint index, GLint size, GLsizei stride, GLuint offset)
{
    // Set access parameters to VBO
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(offset));
    // Enable using of attribute
    glEnableVertexAttribArray(index);
}
