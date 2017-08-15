#include <Buffers/UniformBuffer.h>
#include <cstring>

//#define BUFFER_USE_MAPPING

CUniformBuffer::CUniformBuffer(GLsizeiptr size, GLuint binding)
    : _ubo(0)
    , _binding(binding)
    , _size(size)
{
    glGenBuffers(1, &_ubo);

    bind();
    glBufferData(GL_UNIFORM_BUFFER, _size, nullptr, GL_DYNAMIC_DRAW);
    reset();
    glBindBufferBase(GL_UNIFORM_BUFFER, _binding, _ubo);
    unbind();
}

CUniformBuffer::~CUniformBuffer()
{
    glDeleteBuffers(1, &_ubo);
}

void CUniformBuffer::bind() const
{
    glBindBufferBase(GL_UNIFORM_BUFFER, _binding, _ubo);
}

void CUniformBuffer::unbind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void CUniformBuffer::setData(const void *data) const
{
    bind();
#ifndef BUFFER_USE_MAPPING
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
#ifndef BUFFER_USE_MAPPING
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
#else
    uint8_t *buffer = static_cast<uint8_t *>(glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY));
    memcpy(buffer + offset, data, size);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
#endif
    unbind();
}

void CUniformBuffer::reset() const
{
    void *buffer = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memset(buffer, 0, _size);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
}
