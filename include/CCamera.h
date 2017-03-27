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
    inline bool hasFlag(e_type a) { return static_cast<ul_type>(a); } \
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

    /**
     * @brief The position of the camera
     */
    const glm::vec3& position() const { return _position; }
    void setPosition(const glm::vec3& position);

    /**
     * @brief Offsets the cameras position
     * @param offset    offset direction
     */
    void offsetPosition(const glm::vec3& offset);

    /**
     * @brief Offsets the cameras orientation
     * @param horizontalAngle   the angle (in radians) to offset rightwards (negative values are leftwards)
     * @param verticalAngle     the angle (in radians) to offset downwards (negative values are upwards)
     */
    void offsetOrientation(float horizontalAngle, float verticalAngle);

    /**
     * @brief Orients the camera so that is it directly facing `position`
     * @param position  the position to look at
     */
    void lookAt(const glm::vec3 &position);

    void setViewportAspectRatio(float aspectRatio);

    /**
     * @brief The translation and rotation matrix of the camera
     */
    const glm::mat4 &view() const { return _view; }

    /**
     * @brief The perspective projection transformation matrix
     */
    const glm::mat4 &projection() const { return _projection; }

    glm::mat4 viewProjection() const { return projection() * view(); }

    const glm::vec3 &forward() const { return _forward; }
    const glm::vec3 &right() const { return _right; }
    const glm::vec3 &up() const { return _up; }

private:
    glm::vec3 _position;

    glm::vec3 _forward;
    glm::vec3 _right;
    glm::vec3 _up;

    glm::mat4 _view;
    glm::mat4 _projection;

    float _horizontalAngle;
    float _verticalAngle;

    void updateOrientation();
    void updateViewMatrix();
    void normalizeAngles();
};

#endif // CCAMERA_H
