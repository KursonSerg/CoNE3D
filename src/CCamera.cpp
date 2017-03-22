#include "CCamera.h"

CCamera::CCamera()
    : _position(4.0f, 3.0f, 3.0f)
    , _horizontalAngle(3.14f)
    , _verticalAngle(0.0f)
    , _mouseSpeed(0.0005f)
//    , _viewMatrix( glm::lookAt( _position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0) ) )
{
    LookAt(0.0f, 0.0f);
}

void CCamera::LookAt(float horizontalAngle, float verticalAngle)
{
    _horizontalAngle += _mouseSpeed * horizontalAngle;
    _verticalAngle   += _mouseSpeed * verticalAngle;

    _direction = glm::vec3(
                glm::cos(_verticalAngle) * glm::sin(_horizontalAngle),
                glm::sin(_verticalAngle),
                glm::cos(_verticalAngle) * glm::cos(_horizontalAngle)
                );

    _right = glm::vec3(
                glm::sin(_horizontalAngle - 3.14f/2.0f),
                0,
                glm::cos(_horizontalAngle - 3.14f/2.0f)
                );

    _up = glm::cross(_right, _direction);

    updateViewMatrix();
}

const glm::mat4 &CCamera::getViewMatrix() const
{
    return _viewMatrix;
}

void CCamera::setViewMatrix(const glm::mat4 &viewMatrix)
{
    _viewMatrix = viewMatrix;
}

void CCamera::updateViewMatrix()
{
    _viewMatrix = glm::lookAt(_position, _position + _direction, _up);
}

void CCamera::Move(const EDirection &direction, float deltaTime)
{
    float speed = 5.0f;

    if (has_flag(direction & EDirection::Forward)) {
        _position += _direction * deltaTime * speed;
    }
    if (has_flag(direction & EDirection::Backward)) {
        _position -= _direction * deltaTime * speed;
    }
    if (has_flag(direction & EDirection::Left)) {
        _position -= _right * deltaTime * speed;
    }
    if (has_flag(direction & EDirection::Right)) {
        _position += _right * deltaTime * speed;
    }

    updateViewMatrix();
}
