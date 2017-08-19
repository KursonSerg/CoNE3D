#include <Lights/LightRenderer.h>

CLightRenderer::CLightRenderer()
    : _forceUpdate(false)
{
}

void CLightRenderer::addLight(const std::shared_ptr<CLight> &light)
{
    if (light) {
        _lights.push_back(light);
        _forceUpdate = true;
    }
}

void CLightRenderer::render()
{
    unsigned index = 0;
    for (auto it = _lights.begin(); it != _lights.end();)
    {
        if (auto light = it->lock()) {
            _lightBuffers.setLightBuffer(index, light->_buffer, _forceUpdate);
            ++it; ++index;
        } else {
            it = _lights.erase(it);
            _forceUpdate = true;
        }
    }
    if (_forceUpdate) {
        _lightBuffers.setLightsNumber(static_cast<GLuint>(_lights.size()));
        _forceUpdate = false;
    }
}
