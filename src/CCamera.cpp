#include <CCamera.h>
#include <CLogger.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

CCamera::CCamera(const glm::vec3 &position, const glm::vec3 &look)
    : _position(position)
    , _up(0.0f, 1.0f, 0.0f)
{
    lookAt(look);
}

void CCamera::setPosition(const glm::vec3 &position)
{
    move(position - _position);
}

void CCamera::move(const glm::vec3 &offset)
{
    setViewMatrix(glm::translate(_viewMatrix, -offset));
    _position += offset;
    _buffer.setPosition(_position);
}

void CCamera::rotate(float angle, const glm::vec3 &axis)
{
    _viewMatrix = glm::translate(_viewMatrix, _position);
    _viewMatrix = glm::rotate(_viewMatrix, -angle, axis);
    _direction = glm::rotate(_direction, angle, axis);
    _right = glm::rotate(_right, angle, axis);
    _up = glm::cross(_right, _direction);
    setViewMatrix(glm::translate(_viewMatrix, -_position));
}

void CCamera::roll(float angle)
{
    rotate(angle, _direction);
}

void CCamera::pitch(float angle)
{
    rotate(angle, _right);
}

void CCamera::yaw(float angle)
{
    rotate(angle, _up);
}

void CCamera::lookAt(const glm::vec3 &position)
{
    assert(position != _position);
    _direction = glm::normalize(position - _position);
    _right = glm::cross(_direction, _up);
    setViewMatrix(glm::lookAt(_position, position, _up));
}

void CCamera::setAspectRatio(float aspectRatio)
{
    _projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);
    _buffer.setProjectionMatrix(_projectionMatrix);
}

void CCamera::setViewMatrix(const glm::mat4 &viewMatrix)
{
    _viewMatrix = viewMatrix;
    _buffer.setViewMatrix(_viewMatrix);
}
