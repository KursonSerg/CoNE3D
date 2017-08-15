#include <Drawables/Drawable.h>

CDrawable::CDrawable(GLenum drawMode, GLsizei elementsCount)
    : _drawMode(drawMode)
    , _elementsCount(elementsCount)
{

}

void CDrawable::updateVertices(GLsizeiptr size, const void *data, GLenum drawType)
{
    _vao.bind();

    _vbo.bind();
    _vbo.updateData(size, data, drawType);
}

void CDrawable::updateAttrib(GLuint index, GLint size, GLsizei stride, GLuint offset)
{
    _vbo.updateAttrib(index, size, stride, offset);
}

void CDrawable::render() const
{
    _vao.bind();
    glDrawArrays(_drawMode, 0, _elementsCount);
}
