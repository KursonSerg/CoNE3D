#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <Lights/Light.h>

class CPointLight : public CLight
{
public:
    CPointLight(const glm::vec3 &position);

    void setPosition(const glm::vec3 &position);
};

#endif // POINTLIGHT_H
