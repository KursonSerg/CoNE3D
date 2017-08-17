#include <Lights/Light.h>

CLight::CLight(int type)
{
    _buffer.setType(type);
}

void CLight::setColor(const glm::vec3 &color)
{
    _buffer.setColor(color);
}

void CLight::setAmbientIntensity(float intensity)
{
    _buffer.setAmbientIntensity(intensity);
}

void CLight::setDiffuseIntensity(float intensity)
{
    _buffer.setDiffuseIntensity(intensity);
}
