#include <Lights/PointLight.h>

CPointLight::CPointLight(const glm::vec3 &position)
    : CLight(LIGHT_POINT)
{
    setPosition(position);
}

void CPointLight::setPosition(const glm::vec3 &position)
{
    _buffer.setPosition(position);
}
