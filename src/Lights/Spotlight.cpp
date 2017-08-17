#include <Lights/Spotlight.h>

CSpotlight::CSpotlight(const glm::vec3 &position, const glm::vec3 &direction, float coneAngle)
    : CLight(LIGHT_SPOTLIGHT)
{
    setPosition(position);
    setDirection(direction);
    setConeAngle(coneAngle);
}

void CSpotlight::setPosition(const glm::vec3 &position)
{
    _buffer.setPosition(position);
}

void CSpotlight::setDirection(const glm::vec3 &direction)
{
    _buffer.setDirection(direction);
}

void CSpotlight::setConeAngle(float angle)
{
    _buffer.setConeAngle(angle);
}
