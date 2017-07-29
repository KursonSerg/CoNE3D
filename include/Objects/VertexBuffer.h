#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <Common.h>

class CVertexBuffer
{
public:
    CVertexBuffer();
    ~CVertexBuffer();

    void bind();
    void unbind();

    void updateData(GLsizeiptr size, const void *data, GLenum usage);
    void updateAttrib(GLuint index, GLint size, GLsizei stride, GLuint offset);

private:
    GLuint _vbo;
};

#endif // VERTEXBUFFER_H
