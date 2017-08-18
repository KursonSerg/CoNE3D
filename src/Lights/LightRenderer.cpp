#include <Lights/LightRenderer.h>

CLightRenderer::CLightRenderer()
{
}

void CLightRenderer::addLight(const std::shared_ptr<CLight> &light)
{
    if (light) {
        _lights.push_back(light);
    }
}

void CLightRenderer::render()
{
    unsigned index = 0;
    for (auto it = _lights.begin(); it != _lights.end();)
    {
        if (auto light = it->lock()) {
            _lightBuffers.setLightBuffer(index, light->_buffer);
            ++it; ++index;
        } else {
            it = _lights.erase(it);
        }
    }
    _lightBuffers.setLightsNumber(static_cast<GLuint>(_lights.size()));
}
