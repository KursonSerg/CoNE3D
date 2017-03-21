#ifndef CCAMERA_H
#define CCAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// https://bitbucket.org/martinhofernandes/wheels/src/17aee21522ce8d07c7a74b138e528fadf04d62ed/include/wheels/enums.h++?at=default&fileviewer=file-view-default
// https://github.com/grisumbras/enum-flags/blob/master/include/flags/flags.hpp
// http://stackoverflow.com/a/18554839
// https://www.reddit.com/r/cpp/comments/14oqo9/a_nice_helper_function_for_c11s_enum_classes/

#define FLAGS_UNARY_OPERATOR(OP, e_type) \
    inline constexpr e_type operator OP(e_type const& value) { \
        return static_cast<e_type>(OP static_cast<ul_type>(value)); \
    }

#define FLAGS_BINARY_OPERATOR(OP, e_type) \
    inline constexpr e_type operator OP(e_type const& lhs, e_type const& rhs) { \
        return static_cast<e_type>(static_cast<ul_type>(lhs) OP static_cast<ul_type>(rhs)); \
    } \
    inline e_type& operator OP##=(e_type& lhs, e_type const& rhs) { \
        return lhs = lhs OP rhs; \
    }

#define ALLOW_FLAGS_FOR_ENUM(e_type) \
    typedef std::underlying_type<e_type>::type ul_type; \
    inline bool has_flag(e_type a) { return static_cast<ul_type>(a); } \
    FLAGS_UNARY_OPERATOR(~, e_type) \
    FLAGS_BINARY_OPERATOR(|, e_type) \
    FLAGS_BINARY_OPERATOR(&, e_type) \
    FLAGS_BINARY_OPERATOR(^, e_type)

enum class EDirection
{
    No       = 0,
    Forward  = 1,
    Backward = 2,
    Left     = 4,
    Right    = 8
};

ALLOW_FLAGS_FOR_ENUM(EDirection)

class CCamera
{
public:
    CCamera();

    void LookAt(float horizontal_angle, float vertical_angle);
    const glm::mat4 &GetViewMatrix() { return m_view_matrix; }

    void update()
    {
        glm::vec3 up = glm::cross(m_right, m_direction);
        m_view_matrix = glm::lookAt(m_position, m_position + m_direction, up);
    }

    glm::vec3 m_position;
    glm::vec3 m_right;
    glm::vec3 m_direction;
private:

    float m_horizontal_angle;
    float m_vertical_angle;
    float m_mouse_speed;

    glm::mat4 m_view_matrix;
};

#endif // CCAMERA_H
