#include "CCamera.h"

CCamera::CCamera()
    : m_position(0.0f, 0.0f, 10.0f)
    , m_horizontal_angle(3.14f)
    , m_vertical_angle(0.0f)
    , m_mouse_speed(0.0005f)
    , m_view_matrix( glm::lookAt( m_position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0) ) )
{
    LookAt(0, 0);
}

void CCamera::LookAt(float horizontal_angle, float vertical_angle)
{
    m_horizontal_angle += m_mouse_speed * horizontal_angle;
    m_vertical_angle   += m_mouse_speed * vertical_angle;

    m_direction = glm::vec3(
                glm::cos(m_vertical_angle) * glm::sin(m_horizontal_angle),
                glm::sin(m_vertical_angle),
                glm::cos(m_vertical_angle) * glm::cos(m_horizontal_angle)
                );

    m_right = glm::vec3(
                glm::sin(m_horizontal_angle - 3.14f/2.0f),
                0,
                glm::cos(m_horizontal_angle - 3.14f/2.0f)
                );

    glm::vec3 up = glm::cross(m_right, m_direction);

    m_view_matrix = glm::lookAt(m_position, m_position + m_direction, up);
}
