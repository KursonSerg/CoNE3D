#include <Material.h>

void CMaterial::setDiffuseTexture(CTexture *texture)
{
    _textures[TU_DIFFUSE].reset(texture);
}

void CMaterial::setNormalTexture(CTexture *texture)
{
    _textures[TU_NORMAL].reset(texture);
}

void CMaterial::bind() const
{
    for (unsigned j = 0; j < _textures.size(); ++j)
    {
        if (_textures[j])
        {
            glActiveTexture(GL_TEXTURE0 + j);
            _textures[j]->bind();
        }
    }
    glActiveTexture(GL_TEXTURE0);
}
