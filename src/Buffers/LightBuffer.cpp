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

struct alignas(16) SLightBuffers
{
    SLightBuffer light[MAX_LIGHTS]; // 0
    GLuint lightsNumber;            // 64 * MAX_LIGHTS
}; // 64 * MAX_LIGHTS + 16

CLightBuffer::CLightBuffer()
    : _data(sizeof(SLightBuffer))
    , _changed(false)
{
}

void CLightBuffer::setType(int type)
{
    setData(0, &type, sizeof(int));
}

void CLightBuffer::setAmbientIntensity(float intensity)
{
    setData(4, &intensity, sizeof(float));
}

void CLightBuffer::setDiffuseIntensity(float intensity)
{
    setData(8, &intensity, sizeof(float));
}

void CLightBuffer::setConeAngle(float angle)
{
    setData(12, &angle, sizeof(float));
}

void CLightBuffer::setPosition(const glm::vec3 &position)
{
    setData(16, glm::value_ptr(position), sizeof(glm::vec3));
}

void CLightBuffer::setDirection(const glm::vec3 &direction)
{
    setData(32, glm::value_ptr(direction), sizeof(glm::vec3));
}

void CLightBuffer::setColor(const glm::vec3 &color)
{
    setData(48, glm::value_ptr(color), sizeof(glm::vec3));
}

void CLightBuffer::setData(GLintptr offset, const void *data, GLsizeiptr size)
{
    memcpy(getData() + offset, data, size);
    _changed = true;
}

CLightBuffers::CLightBuffers()
    : CUniformBuffer(sizeof(SLightBuffers), BUFFER_LIGHT_BINDING)
{
}

void CLightBuffers::setLightBuffer(unsigned index, CLightBuffer &buffer, bool force)
{
    if (buffer._changed || force) {
        setSubData(sizeof(SLightBuffer) * index, sizeof(SLightBuffer), buffer.getData());
        buffer._changed = false;
    }
}

void CLightBuffers::setLightsNumber(GLuint number)
{
    setSubData(sizeof(SLightBuffer) * MAX_LIGHTS, sizeof(GLuint), &number);
}
