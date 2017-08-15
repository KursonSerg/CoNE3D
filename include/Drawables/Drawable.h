#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <Buffers/VertexArray.h>
#include <Buffers/VertexBuffer.h>

class CDrawable
{
public:
    CDrawable(GLenum drawMode, GLsizei elementsCount);

    void updateVertices(GLsizeiptr size, const void *data, GLenum drawType);
    void updateAttrib(GLuint index, GLint size, GLsizei stride, GLuint offset);

    virtual void render() const;

protected:
    CVertexArray _vao;
    CVertexBuffer _vbo;

    GLenum _drawMode;
    GLsizei _elementsCount;
};

#endif // DRAWABLE_H
