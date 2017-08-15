#include <Drawables/IndexedDrawable.h>

CIndexedDrawable::CIndexedDrawable(GLenum drawMode, GLsizei elementsCount, GLenum indicesType)
    : CDrawable(drawMode, elementsCount)
    , _indicesType(indicesType)
{
}

void CIndexedDrawable::updateIndices(GLsizeiptr size, const void *data, GLenum drawType)
{
    _ibo.bind();
    _ibo.updateData(size, data, drawType);
}

void CIndexedDrawable::render() const
{
    _vao.bind();
    glDrawElements(_drawMode, _elementsCount, _indicesType, nullptr);
}
