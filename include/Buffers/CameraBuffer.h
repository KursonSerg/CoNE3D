#ifndef CAMERABUFFER_H
#define CAMERABUFFER_H

#include <Buffers/UniformBuffer.h>

#include <glm/glm.hpp>

class CCameraBuffer final : private CUniformBuffer
{
public:
    CCameraBuffer();

    void setProjectionMatrix(const glm::mat4 &matrix) const;
    void setViewMatrix(const glm::mat4 &matrix) const;
    void setPosition(const glm::vec3 &position) const;
};

#endif // CAMERABUFFER_H
