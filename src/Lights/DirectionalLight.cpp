#include <Lights/DirectionalLight.h>

CDirectionalLight::CDirectionalLight(const glm::vec3 &direction)
    : CLight(LIGHT_DIRECTIONAL)
{
    setDirection(direction);
}

void CDirectionalLight::setDirection(const glm::vec3 &direction)
{
    _buffer.setDirection(direction);
}
