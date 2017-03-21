#ifndef COBJECT_H
#define COBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CObject
{
public:
    CObject();

    void setScale(const glm::vec3 &scale);
    void setPosition(const glm::vec3 &position);
    void setRotation(const glm::mat4 &rotation);

private:
    glm::vec3 _scale;
    glm::vec3 _position;
    glm::mat4 _rotation;
};

#endif // COBJECT_H
