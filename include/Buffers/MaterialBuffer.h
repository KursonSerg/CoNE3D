#ifndef MATERIALBUFFER_H
#define MATERIALBUFFER_H

#include <Buffers/UniformBuffer.h>

#include <glm/glm.hpp>

class CMaterialBuffer final : public CUniformBuffer
{
public:
    CMaterialBuffer();

    void setDiffuseColor(const glm::vec3 &color) const;
    void setSpecularColor(const glm::vec3 &color) const;
    void setAmbientColor(const glm::vec3 &color) const;

    void setUseDiffuseTexture(GLint use);
    void setUseSpecularTexture(GLint use);
    void setUseNormalTexture(GLint use);

    void setShininess(GLfloat shininess);
};

#endif // MATERIALBUFFER_H
