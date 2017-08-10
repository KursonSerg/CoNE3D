#include <Buffers/UniformBuffer.h>
#include <cstring>

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
#if 1
    glBufferSubData(GL_UNIFORM_BUFFER, 0, _size, data);
#else
    void *buffer = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(buffer, data, _size);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
#endif
    unbind();
}

void CUniformBuffer::setSubData(GLintptr offset, GLsizeiptr size, const void *data) const
{
    bind();
#if 1
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
#else
    uint8_t *buffer = static_cast<uint8_t *>(glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY));
    memcpy(buffer + offset, data, size);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
#endif
    unbind();
}
