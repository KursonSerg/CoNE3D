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
    CProgram  m_program;
    CShader   m_vertex_shader;
    CShader   m_fragment_shader;

    GLuint    m_mesh_vao;
    GLuint    m_mesh_vbo;
};

#endif // CMESH_H
