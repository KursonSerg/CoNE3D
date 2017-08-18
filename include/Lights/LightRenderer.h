#ifndef LIGHTRENDERER_H
#define LIGHTRENDERER_H

#include <Lights/DirectionalLight.h>
#include <Lights/PointLight.h>
#include <Lights/Spotlight.h>

class CLightRenderer
{
public:
    CLightRenderer();

    void addLight(const std::shared_ptr<CLight> &light);
    void render();

private:
    CLightBuffers _lightBuffers;
    std::list<std::weak_ptr<CLight>> _lights;
};

#endif // LIGHTRENDERER_H
