#ifndef LIGHTBUFFER_H
#define LIGHTBUFFER_H

#include <Buffers/UniformBuffer.h>
#include <Defines.glsl>

#include <vector>
#include <glm/glm.hpp>

class CLightBuffer final
{
public:
    CLightBuffer();

    void setType(int type);
    void setAmbientIntensity(float intensity);
    void setDiffuseIntensity(float intensity);
    void setConeAngle(float angle);
    void setPosition(const glm::vec3 &position);
    void setDirection(const glm::vec3 &direction);
    void setColor(const glm::vec3 &color);

private:
    std::vector<uint8_t> _data;
    bool _changed;

    uint8_t *getData()
    {
        return _data.data();
    }
    void setData(GLintptr offset, const void *data, GLsizeiptr size);

    friend class CLightBuffers;
};

class CLightBuffers final : public CUniformBuffer
{
public:
    CLightBuffers();

    void setLightBuffer(unsigned index, CLightBuffer &buffer, bool force);
    void setLightsNumber(GLuint number);
};

#endif // LIGHTBUFFER_H
