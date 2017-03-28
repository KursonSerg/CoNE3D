#include <CMesh.h>

// Count of vertices
static const uint32_t MESH_VERTEX_COUNT      = 36;

// Size of one vertex in bytes: 6 floats for position and color
static const uint32_t VERTEX_SIZE            = 6 * sizeof(GL_FLOAT);

// Data offsets inside vertex
static const uint32_t VERTEX_POSITION_OFFSET = 0;
static const uint32_t VERTEX_COLOR_OFFSET    = 3 * sizeof(GL_FLOAT);

static const GLfloat CUBE_MESH[MESH_VERTEX_COUNT * 6] = {
    -1.0f, -1.0f, -1.0f, 0.583f, 0.771f, 0.014f,
    -1.0f, -1.0f,  1.0f, 0.609f, 0.115f, 0.436f,
    -1.0f,  1.0f,  1.0f, 0.327f, 0.483f, 0.844f,
     1.0f,  1.0f, -1.0f, 0.822f, 0.569f, 0.201f,
    -1.0f, -1.0f, -1.0f, 0.435f, 0.602f, 0.223f,
    -1.0f,  1.0f, -1.0f, 0.310f, 0.747f, 0.185f,
     1.0f, -1.0f,  1.0f, 0.597f, 0.770f, 0.761f,
    -1.0f, -1.0f, -1.0f, 0.559f, 0.436f, 0.730f,
     1.0f, -1.0f, -1.0f, 0.359f, 0.583f, 0.152f,
     1.0f,  1.0f, -1.0f, 0.483f, 0.596f, 0.789f,
     1.0f, -1.0f, -1.0f, 0.559f, 0.861f, 0.639f,
    -1.0f, -1.0f, -1.0f, 0.195f, 0.548f, 0.859f,
    -1.0f, -1.0f, -1.0f, 0.014f, 0.184f, 0.576f,
    -1.0f,  1.0f,  1.0f, 0.771f, 0.328f, 0.970f,
    -1.0f,  1.0f, -1.0f, 0.406f, 0.615f, 0.116f,
     1.0f, -1.0f,  1.0f, 0.676f, 0.977f, 0.133f,
    -1.0f, -1.0f,  1.0f, 0.971f, 0.572f, 0.833f,
    -1.0f, -1.0f, -1.0f, 0.140f, 0.616f, 0.489f,
    -1.0f,  1.0f,  1.0f, 0.997f, 0.513f, 0.064f,
    -1.0f, -1.0f,  1.0f, 0.945f, 0.719f, 0.592f,
     1.0f, -1.0f,  1.0f, 0.543f, 0.021f, 0.978f,
     1.0f,  1.0f,  1.0f, 0.279f, 0.317f, 0.505f,
     1.0f, -1.0f, -1.0f, 0.167f, 0.620f, 0.077f,
     1.0f,  1.0f, -1.0f, 0.347f, 0.857f, 0.137f,
     1.0f, -1.0f, -1.0f, 0.055f, 0.953f, 0.042f,
     1.0f,  1.0f,  1.0f, 0.714f, 0.505f, 0.345f,
     1.0f, -1.0f,  1.0f, 0.783f, 0.290f, 0.734f,
     1.0f,  1.0f,  1.0f, 0.722f, 0.645f, 0.174f,
     1.0f,  1.0f, -1.0f, 0.302f, 0.455f, 0.848f,
    -1.0f,  1.0f, -1.0f, 0.225f, 0.587f, 0.040f,
     1.0f,  1.0f,  1.0f, 0.517f, 0.713f, 0.338f,
    -1.0f,  1.0f, -1.0f, 0.053f, 0.959f, 0.120f,
    -1.0f,  1.0f,  1.0f, 0.393f, 0.621f, 0.362f,
     1.0f,  1.0f,  1.0f, 0.673f, 0.211f, 0.457f,
    -1.0f,  1.0f,  1.0f, 0.820f, 0.883f, 0.371f,
     1.0f, -1.0f,  1.0f, 0.982f, 0.099f, 0.879f
};

CMesh::CMesh()
    : _vao(0)
    , _vbo(0)
{
    GLuint positionLocation = 0, colorLocation = 1;

    _program.AttachShader(CShader(EShaderType::Vertex, "assets/simple.vs"));
    _program.AttachShader(CShader(EShaderType::Fragment, "assets/simple.fs"));

    CProgram::AttribInfo attribs;
    attribs[positionLocation] = "position";
    attribs[colorLocation] = "color";
    _program.BindAttrib(attribs);

    // Create a Vertex Array Object (VAO) and set it as the current one
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // Create a Vertex Buffer Object (VBO) and set it as the current one
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    // Fill VBO with vertices
    glBufferData(GL_ARRAY_BUFFER, MESH_VERTEX_COUNT * VERTEX_SIZE, CUBE_MESH, GL_STATIC_DRAW);

    // Set access parameters to VBO
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(VERTEX_POSITION_OFFSET));
    // Enable using of attribute
    glEnableVertexAttribArray(positionLocation);

    // Set access parameters to VBO
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(VERTEX_COLOR_OFFSET));
    // Enable using of attribute
    glEnableVertexAttribArray(colorLocation);

    _program.Link();
    _program.Use();
    _program.Validate();
}

CMesh::~CMesh()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &_vbo);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &_vao);
}

void CMesh::Render(const glm::mat4 &mvpMatrix)
{
    // Set shader program as the active one
    _program.Use();

    // Set perspective matrix in shader
    GLint mvpMatrixLocation = _program.GetUniform("mvpMatrix");
    glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

    // Using VAO for rendering
    glBindVertexArray(_vao);
    // Render vertices in VBO binded to VAO
    glDrawArrays(GL_TRIANGLES, 0, MESH_VERTEX_COUNT);
}
