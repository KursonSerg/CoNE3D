#ifndef LIGHT_H
#define LIGHT_H

#include <Buffers/LightBuffer.h>

class CLight
{
public:
    CLight(int type);

    void setColor(const glm::vec3 &color);
    void setAmbientIntensity(float intensity);
    void setDiffuseIntensity(float intensity);

protected:
    CLightBuffer _buffer;
    friend class CLightRenderer;
};

#endif // LIGHT_H
