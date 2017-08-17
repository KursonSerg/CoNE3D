#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <Lights/Light.h>

class CSpotlight : public CLight
{
public:
    CSpotlight(const glm::vec3 &position, const glm::vec3 &direction, float coneAngle);

    void setPosition(const glm::vec3 &position);
    void setDirection(const glm::vec3 &direction);
    void setConeAngle(float angle);
};

#endif // SPOTLIGHT_H
