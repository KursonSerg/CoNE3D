#include <Buffers/LightBuffer.h>

#include <glm/gtc/type_ptr.hpp>

struct alignas(16) SLightBuffer
{
    int type;               // 0
    float ambientIntensity; // 4
    float diffuseIntensity; // 8
    float coneAngle;        // 12
    glm::vec3 position;     // 16
    glm::vec3 direction;    // 32
    glm::vec3 color;        // 48
}; // 64

CLightBuffer::CLightBuffer()
    : CUniformBuffer(sizeof(SLightBuffer), BUFFER_LIGHT_BINDING)
{
}

void CLightBuffer::setType(int type)
{
    setSubData(0, sizeof(int), &type);
}

void CLightBuffer::setAmbientIntensity(float intensity)
{
    setSubData(4, sizeof(float), &intensity);
}

void CLightBuffer::setDiffuseIntensity(float intensity)
{
    setSubData(8, sizeof(float), &intensity);
}

void CLightBuffer::setConeAngle(float angle)
{
    setSubData(12, sizeof(float), &angle);
}

void CLightBuffer::setPosition(const glm::vec3 &position)
{
    setSubData(16, sizeof(glm::vec3), glm::value_ptr(position));
}

void CLightBuffer::setDirection(const glm::vec3 &direction)
{
    setSubData(32, sizeof(glm::vec3), glm::value_ptr(direction));
}

void CLightBuffer::setColor(const glm::vec3 &color)
{
    setSubData(48, sizeof(glm::vec3), glm::value_ptr(color));
}
