#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Buffers/CameraBuffer.h>

class CCamera
{
public:
    CCamera(const glm::vec3 &position, const glm::vec3 &look);

    /**
     * @brief Gets the position of the camera
     */
    const glm::vec3 &getPosition() const { return _position; }
    /**
     * @brief Sets the position of the camera
     * @param[in] position    New position
     */
    void setPosition(const glm::vec3 &position);

    /**
     * @brief Moves the camera
     * @param[in] offset    Move direction
     */
    void move(const glm::vec3 &offset);

    /**
     * @brief Rotates the camera by the `angle` around the `axis`
     * @param[in] angle    Rotation angle
     * @param[in] axis    Rotation axis
     */
    void rotate(float angle, const glm::vec3 &axis);
    void roll(float angle);
    void pitch(float angle);
    void yaw(float angle);

    /**
     * @brief Orients the camera so that is it directly facing `position`
     * @param position  New position to look at
     */
    void lookAt(const glm::vec3 &position);

    void setAspectRatio(float aspectRatio);

    /**
     * @brief Gets the translation and rotation matrix of the camera
     */
    const glm::mat4 &getViewMatrix() const { return _viewMatrix; }

    /**
     * @brief Gets the perspective projection transformation matrix
     */
    const glm::mat4 &getProjectionMatrix() const { return _projectionMatrix; }

    glm::mat4 viewProjection() const { return _projectionMatrix * _viewMatrix; }

    const glm::vec3 &getDirection() const { return _direction; }
    const glm::vec3 &getRight() const { return _right; }
    const glm::vec3 &getUp() const { return _up; }

private:
    glm::vec3 _position;

    glm::vec3 _direction;
    glm::vec3 _right;
    glm::vec3 _up;

    glm::mat4 _viewMatrix;
    glm::mat4 _projectionMatrix;

    void setViewMatrix(const glm::mat4 &getViewMatrix);

    CCameraBuffer _buffer;
};

#endif // CAMERA_H
