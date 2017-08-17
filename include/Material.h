#ifndef MATERIAL_H
#define MATERIAL_H

#include <array>
#include <glm/glm.hpp>

#include <CTexture.h>
#include <Buffers/MaterialBuffer.h>

class CMaterial final
{
public:
    void setDiffuseTexture(CTexture *texture);
    void setNormalTexture(CTexture *texture);

    void setDiffuseColor(const glm::vec3 &color) const;
    void setSpecularColor(const glm::vec3 &color) const;
    void setAmbientColor(const glm::vec3 &color) const;

    void setShininess(GLfloat shininess);

    void bind() const;

private:
    std::array<std::unique_ptr<CTexture>, MAX_TEXTURE_UNITS> _textures;

    CMaterialBuffer _buffer;
};

#endif // MATERIAL_H
