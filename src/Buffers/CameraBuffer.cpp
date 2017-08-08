#include <Buffers/CameraBuffer.h>

#include <glm/gtc/type_ptr.hpp>

struct alignas(16) SCameraBuffer
{
    glm::mat4 viewMatrix;       // 0
    glm::mat4 projectionMatrix; // 64
    glm::vec3 position;         // 128
};

CCameraBuffer::CCameraBuffer()
    : CUniformBuffer(sizeof(SCameraBuffer), 0) // TODO: Define bindings for uniform buffers
{
}

void CCameraBuffer::setProjectionMatrix(const glm::mat4 &matrix) const
{
    setSubData(0, sizeof(glm::mat4), glm::value_ptr(matrix));
}

void CCameraBuffer::setViewMatrix(const glm::mat4 &matrix) const
{
    setSubData(64, sizeof(glm::mat4), glm::value_ptr(matrix));
}

void CCameraBuffer::setPosition(const glm::vec3 &position) const
{
    setSubData(128, sizeof(glm::vec3), glm::value_ptr(position));
}
