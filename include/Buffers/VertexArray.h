#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <Common.h>

class CVertexArray final
{
public:
    CVertexArray();
    ~CVertexArray();

    void bind() const;
    void unbind() const;

private:
    GLuint _vao;
};

#endif // VERTEXARRAY_H
