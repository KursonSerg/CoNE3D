#include <Material.h>

void CMaterial::setDiffuseTexture(CTexture *texture)
{
    _textures[TU_DIFFUSE].reset(texture);
    _buffer.setUseDiffuseTexture(texture != nullptr);
}

void CMaterial::setNormalTexture(CTexture *texture)
{
    _textures[TU_NORMAL].reset(texture);
    _buffer.setUseNormalTexture(texture != nullptr);
}

void CMaterial::setDiffuseColor(const glm::vec3 &color) const
{
    _buffer.setDiffuseColor(color);
}

void CMaterial::setSpecularColor(const glm::vec3 &color) const
{
    _buffer.setSpecularColor(color);
}

void CMaterial::setAmbientColor(const glm::vec3 &color) const
{
    _buffer.setAmbientColor(color);
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
    _buffer.bind();
}
