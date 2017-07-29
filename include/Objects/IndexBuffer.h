#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <Common.h>

class CIndexBuffer
{
public:
    CIndexBuffer();
    ~CIndexBuffer();

    void bind();
    void unbind();

    void updateData(GLsizeiptr size, const void *data, GLenum usage);

private:
    GLuint _ibo;
};

#endif // INDEXBUFFER_H
