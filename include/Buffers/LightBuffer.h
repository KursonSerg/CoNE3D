#ifndef LIGHTBUFFER_H
#define LIGHTBUFFER_H

#include <Buffers/UniformBuffer.h>
#include <Defines.glsl>

#include <glm/glm.hpp>

class CLightBuffer final : public CUniformBuffer
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
};

#endif // LIGHTBUFFER_H
