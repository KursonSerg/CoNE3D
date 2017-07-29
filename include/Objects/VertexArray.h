#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <Common.h>

class CVertexArray
{
public:
    CVertexArray();
    ~CVertexArray();

    void bind();
    void unbind();

private:
    GLuint _vao;
};

#endif // VERTEXARRAY_H
