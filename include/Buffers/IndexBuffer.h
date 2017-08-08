#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <Common.h>

class CIndexBuffer final
{
public:
    CIndexBuffer();
    ~CIndexBuffer();

    void bind() const;
    void unbind() const;

    void updateData(GLsizeiptr size, const void *data, GLenum usage);

private:
    GLuint _ibo;
};

#endif // INDEXBUFFER_H
