#ifndef CMESH_H
#define CMESH_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <CShader.h>

class CMesh
{
public:
    CMesh();
    ~CMesh();

    void Render(const glm::mat4 &mvp_matrix);

private:
    CProgram  _program;
    CShader   _vertexShader;
    CShader   _fragmentShader;

    GLuint    _vao;
    GLuint    _vbo;
};

#endif // CMESH_H
