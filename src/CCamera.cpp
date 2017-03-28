#include <CCamera.h>
#include <CLogger.h>

CCamera::CCamera()
    : _position(5.0f, 4.0f, 3.0f)
    , _horizontalAngle(0.0f)
    , _verticalAngle(0.0f)
{
    lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
}

void CCamera::setPosition(const glm::vec3 &position)
{
    _position = position;

    updateViewMatrix();
}

void CCamera::offsetPosition(const glm::vec3 &offset)
{
    _position += offset;

    updateViewMatrix();
}

void CCamera::offsetOrientation(float horizontalAngle, float verticalAngle)
{
    _horizontalAngle += horizontalAngle;
    _verticalAngle   += verticalAngle;
    normalizeAngles();

    updateOrientation();
}

void CCamera::lookAt(const glm::vec3 &position)
{
    assert(position != _position);
    glm::vec3 direction = glm::normalize(position - _position);
    _verticalAngle = glm::degrees(asinf(-direction.y));
    _horizontalAngle = glm::degrees(-atan2f(-direction.x, -direction.z));
    normalizeAngles();

    updateOrientation();
}

void CCamera::setViewportAspectRatio(float aspectRatio)
{
    _projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}

void CCamera::updateOrientation()
{
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(_verticalAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    orientation = glm::rotate(orientation, glm::radians(_horizontalAngle), glm::vec3(0.0f, 1.0f, 0.0f));

    _forward = glm::vec3(glm::inverse(orientation) * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
    _right = glm::vec3(glm::inverse(orientation) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    _up = glm::vec3(glm::inverse(orientation) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    // _up = glm::cross(_right, _direction);

    updateViewMatrix();
}

void CCamera::updateViewMatrix()
{
    _view = glm::lookAt(_position, _position + _forward, _up);
}

void CCamera::normalizeAngles()
{
    static const float maxVerticalAngle = 85.0f;

    _horizontalAngle = std::fmod(_horizontalAngle, 360.0f);
    // fmod can return negative values, but this will make them all positive
    if (_horizontalAngle < 0.0f)
        _horizontalAngle += 360.0f;

    if (_verticalAngle > maxVerticalAngle)
        _verticalAngle = maxVerticalAngle;
    else if (_verticalAngle < -maxVerticalAngle)
        _verticalAngle = -maxVerticalAngle;
}
