#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <Lights/Light.h>

class CDirectionalLight : public CLight
{
public:
    CDirectionalLight(const glm::vec3 &direction);

    void setDirection(const glm::vec3 &direction);
};

#endif // DIRECTIONALLIGHT_H
