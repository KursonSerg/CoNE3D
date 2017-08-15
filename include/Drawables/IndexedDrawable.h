#ifndef INDEXEDDRAWABLE_H
#define INDEXEDDRAWABLE_H

#include <Drawables/Drawable.h>
#include <Buffers/IndexBuffer.h>

class CIndexedDrawable : protected CDrawable
{
public:
    CIndexedDrawable(GLenum drawMode, GLsizei elementsCount, GLenum indicesType);

    void updateIndices(GLsizeiptr size, const void *data, GLenum drawType);

    virtual void render() const override;

private:
    CIndexBuffer _ibo;
    GLenum _indicesType;
};

#endif // INDEXEDDRAWABLE_H
