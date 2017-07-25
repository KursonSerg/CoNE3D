#include <Version.h>
#include <CWindow.h>
#include <CLogger.h>
#include <CMesh.h>
#include <CCamera.h>

class CWindowTest : public CWindow
{
public:
    CWindowTest(int width, int height, const std::wstring &title)
        : CWindow(width, height, title)
        , _camera(glm::vec3(5.0f, 4.0f, 4.0f), glm::vec3(0.0f, 2.0f, 0.0f))
        , _mesh("assets/hulk/hulk.obj")
        , _angle(0.0f)
        , _speed(45.0f)
        , _movementDirection(EDirection::No)
    {
        utils::Log(utils::CFormat(L"CWindowTest::CWindowTest(%%)") << title, utils::ELogLevel::Debug);

        Resize(width, height); // FIXME: Move to another place (but not CWindow constructor)
        Init();

        _simple.AttachShader(CShader(EShaderType::Vertex, "assets/simple.vert"));
        _simple.AttachShader(CShader(EShaderType::Fragment, "assets/simple.frag"));

        _simple.Link();
        _simple.Validate();

        _shading.AttachShader(CShader(EShaderType::Vertex, "assets/normal_mapping.vert"));
        _shading.AttachShader(CShader(EShaderType::Fragment, "assets/normal_mapping.frag"));

        _shading.Link();
        _shading.Validate();

        _shading.LoadUniforms();
    }

    virtual ~CWindowTest()
    {
        utils::Log(utils::CFormat(L"CWindowTest::~CWindowTest()"), utils::ELogLevel::Debug);
    }

    virtual void Init() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual void Resize(int width, int height) override;
    virtual void Key(int key, int action, int mods) override;
    virtual void Cursor(float xpos, float ypos) override;

private:
    CCamera  _camera;
    CProgram _simple;
    CProgram _shading;
    CMesh    _mesh;

    float _angle;
    float _speed;

    EDirection _movementDirection;
};

void CWindowTest::Init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}

void CWindowTest::Update(float deltaTime)
{
    _angle += _speed * deltaTime;
    if (_angle >= 360.0f) {
        _angle = 0.0f;
    }

    const float moveSpeed = 5.0f; // units per second
    if (hasFlag(_movementDirection & EDirection::Forward)) {
        _camera.offsetPosition(deltaTime * moveSpeed * _camera.forward());
    }
    else if (hasFlag(_movementDirection & EDirection::Backward)) {
        _camera.offsetPosition(deltaTime * moveSpeed * -_camera.forward());
    }
    if (hasFlag(_movementDirection & EDirection::Right)) {
        _camera.offsetPosition(deltaTime * moveSpeed * _camera.right());
    }
    else if (hasFlag(_movementDirection & EDirection::Left)) {
        _camera.offsetPosition(deltaTime * moveSpeed * -_camera.right());
    }
}

void CWindowTest::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    bool useLight = true;

    // Set shader program as the active one
    CProgram &current = useLight ? _shading : _simple;

    current.Use();

    {
        // Model matrix: Scale * Rotation * Translation
        glm::mat4 model = glm::rotate( glm::mat4(1.0f), glm::radians(_angle), glm::vec3(0.0f, 1.0f, 0.0f) );

        if (useLight)
        {
            glm::vec3 lightPos = glm::vec3(4.0f, 4.0f, 4.0f);
            glUniform3f(current.GetUniform("LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);

            glUniformMatrix4fv(current.GetUniform("M"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(current.GetUniform("V"), 1, GL_FALSE, glm::value_ptr(_camera.view()));
            glUniformMatrix3fv(current.GetUniform("MV3x3"), 1, GL_FALSE, glm::value_ptr(glm::mat3(_camera.view() * model)));
        }

        // Set model & view & projection matrix in shader
        glUniformMatrix4fv(current.GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(_camera.viewProjection() * model));

        _mesh.Render();
    }

#if 0
    {
        glm::mat4 model = glm::translate( glm::mat4(1.0f), glm::vec3(3.0f, 2.0f, 0.0f) );
        model = glm::rotate( model, glm::radians(_angle), glm::vec3(1.0f, 0.0f, 0.0f) );
        model = glm::scale( model, glm::vec3(0.75f, 0.75f, 0.75f) );
        _cubeMesh.Render(_camera.viewProjection() * model);
    }

    {
        glm::mat4 model = glm::translate( glm::mat4(1.0f), glm::vec3(-3.0f, -2.0f, 0.0f) );
        model = glm::rotate( model, glm::radians(_angle), glm::vec3(0.0f, 0.0f, 1.0f) );
        model = glm::scale( model, glm::vec3(0.75f, 0.75f, 0.75f) );
        _cubeMesh.Render(_camera.viewProjection() * model);
    }
#endif

    current.Unuse();

    CenterMouse();

    utils::checkErrorStatus();
}

void CWindowTest::Resize(int width, int height)
{
    utils::Log(utils::CFormat(L"CWindowTest::Resize(%%x%%)") << width << height, utils::ELogLevel::Debug);

    _camera.setViewportAspectRatio( static_cast<float>(_width / _height) );

    // Set viewport
    glViewport(0, 0, width, height);
}

void CWindowTest::Key(int key, int action, int mods)
{
    UNUSED(mods);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        Destroy();
    }/* else if (key == GLFW_KEY_LEFT) {
        if (action == GLFW_PRESS) {
            _speed = 1.0;
        } else if (action == GLFW_RELEASE && _speed > 0.0) {
            _speed = 0.0;
        }
    } else if (key == GLFW_KEY_RIGHT) {
        if (action == GLFW_PRESS) {
            _speed = -1.0;
        } else if (action == GLFW_RELEASE && _speed < 0.0) {
            _speed = 0.0;
        }
    }*/

    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) {
            _movementDirection |= EDirection::Forward;
        } else if (action == GLFW_RELEASE) {
            _movementDirection &= ~EDirection::Forward;
        }
    } else if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            _movementDirection |= EDirection::Backward;
        } else if (action == GLFW_RELEASE) {
            _movementDirection &= ~EDirection::Backward;
        }
    } else if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            _movementDirection |= EDirection::Right;
        } else if (action == GLFW_RELEASE) {
            _movementDirection &= ~EDirection::Right;
        }
    } else if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
            _movementDirection |= EDirection::Left;
        } else if (action == GLFW_RELEASE) {
            _movementDirection &= ~EDirection::Left;
        }
    }
}

void CWindowTest::Cursor(float xpos, float ypos)
{
    // utils::Log(utils::CFormat(L"CWindowTest::Cursor(%%, %%)") << xpos << ypos, utils::ELogLevel::Debug);

    const float mouseSensitivity = 0.01f;
    _camera.offsetOrientation( (xpos - _width/2.0f) * mouseSensitivity,
                               (ypos - _height/2.0f) * mouseSensitivity );
}

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    utils::Log(utils::CFormat(L"CoNE3D version: %%.%%.%%")
               << CONE3D_VERSION_MAJOR << CONE3D_VERSION_MINOR
               << CONE3D_VERSION_REVISION, utils::ELogLevel::Info);

    try
    {
        // Initialize glfw library
        if ( !glfwInit() ) {
            throw std::runtime_error("Failed to initialize window library");
        }

        CWindowTest window(640, 360, L"CoNE3D Window Test");
        window.Process();

        // Clean up glfw library
        glfwTerminate();
    }
    catch (const std::exception &ex)
    {
        utils::Log(ex.what(), utils::ELogLevel::Error);
    }

    return 0;
}
