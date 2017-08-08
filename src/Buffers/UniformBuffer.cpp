#include <Buffers/UniformBuffer.h>

CUniformBuffer::CUniformBuffer(GLsizeiptr size, GLuint binding)
    : _ubo(0)
    , _size(size)
{
    glGenBuffers(1, &_ubo);

    bind();
    glBufferData(GL_UNIFORM_BUFFER, _size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, _ubo);
    unbind();
}

CUniformBuffer::~CUniformBuffer()
{
    glDeleteBuffers(1, &_ubo);
}

void CUniformBuffer::bind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
}

void CUniformBuffer::unbind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void CUniformBuffer::setData(const void *data) const
{
    bind();
    glBufferSubData(GL_UNIFORM_BUFFER, 0, _size, data);
    unbind();
}

void CUniformBuffer::setSubData(GLintptr offset, GLsizeiptr size, const void *data) const
{
    bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    unbind();
}
