#ifndef CMESH_H
#define CMESH_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <CTexture.h>
#include <CShader.h>

class CMesh
{
public:
    CMesh();
    ~CMesh();

    void Render(const glm::mat4 &mvpMatrix);

private:
    CTexture _texture;
    CProgram _program;

    GLuint _vao;
    GLuint _vbo;
};

#endif // CMESH_H
