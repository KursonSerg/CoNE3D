#include <Buffers/MaterialBuffer.h>

#include <glm/gtc/type_ptr.hpp>

struct alignas(16) SMaterialBuffer
{
    glm::vec4 diffuseColor;     // 0
    glm::vec4 specularColor;    // 16
    glm::vec4 ambientColor;     // 32

    GLint useDiffuseTexture;    // 48
    GLint useSpecularTexture;   // 52
    GLint useNormalTexture;     // 56

    GLfloat shininess;          // 60
}; // 64

CMaterialBuffer::CMaterialBuffer()
    : CUniformBuffer(sizeof(SMaterialBuffer), BUFFER_MATERIAL_BINDING)
{
}

void CMaterialBuffer::setDiffuseColor(const glm::vec3 &color) const
{
    setSubData(0, sizeof(glm::vec3), glm::value_ptr(color));
}

void CMaterialBuffer::setSpecularColor(const glm::vec3 &color) const
{
    setSubData(16, sizeof(glm::vec3), glm::value_ptr(color));
}

void CMaterialBuffer::setAmbientColor(const glm::vec3 &color) const
{
    setSubData(32, sizeof(glm::vec3), glm::value_ptr(color));
}

void CMaterialBuffer::setUseDiffuseTexture(GLint use)
{
    setSubData(48, sizeof(GLint), &use);
}

void CMaterialBuffer::setUseSpecularTexture(GLint use)
{
    setSubData(52, sizeof(GLint), &use);
}

void CMaterialBuffer::setUseNormalTexture(GLint use)
{
    setSubData(56, sizeof(GLint), &use);
}

void CMaterialBuffer::setShininess(GLfloat shininess)
{
    setSubData(60, sizeof(GLfloat), &shininess);
}
