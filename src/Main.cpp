#include <Version.h>
#include <CWindow.h>
#include <CLogger.h>
#include <CModel.h>
#include <CCamera.h>

#include <Lights/DirectionalLight.h>
#include <Lights/PointLight.h>
#include <Lights/Spotlight.h>
#include <Shaders/Program.h>

class CWindowTest : public CWindow
{
public:
    CWindowTest(int width, int height, const std::wstring &title)
        : CWindow(width, height, title)
        , _camera(glm::vec3(2.0f, 4.0f, 6.0f), glm::vec3(0.0f, 3.5f, 0.0f))
        , _model("assets/hulk/hulk.obj")
        , _light(glm::vec3(2.0f, 4.0f, 6.0f), glm::vec3(-0.5f, 0.0f, -1.5f), 10.0f)
        , _rotateAngle(0.0f)
        , _rotateSpeed(0.0f)
    {
        utils::Log(utils::CFormat(L"CWindowTest::CWindowTest(%%)") << title, utils::ELogLevel::Debug);

        Resize(width, height); // FIXME: Move to another place (but not CWindow constructor)
        Init();

        _simple.loadShaders("glsl/Basic.glsl");
        _simple.loadUniforms();
        _simple.loadUniformBlocks();
        _shading.loadShaders("glsl/Shading.glsl");
        _shading.loadUniforms();
        _shading.loadUniformBlocks();
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
    CModel   _model;
    CSpotlight _light;

    float _rotateAngle;
    float _rotateSpeed;
};

void CWindowTest::Init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);

    _light.setAmbientIntensity(0.5f);
    _light.setDiffuseIntensity(50.0f);
    _light.setColor(glm::vec3(1.0f));
}

void CWindowTest::Update(float deltaTime)
{
    _rotateAngle += _rotateSpeed * deltaTime;
    if (_rotateAngle >= 360.0f) {
        _rotateAngle = 0.0f;
    }

    static const float moveSpeed = 5.0f; // units per second

    if (GetKeyState(GLFW_KEY_W) == GLFW_PRESS) {
        _camera.move(deltaTime * moveSpeed * _camera.getDirection());
    } else if (GetKeyState(GLFW_KEY_S) == GLFW_PRESS) {
        _camera.move(deltaTime * moveSpeed * -_camera.getDirection());
    } else if (GetKeyState(GLFW_KEY_D) == GLFW_PRESS) {
        _camera.move(deltaTime * moveSpeed * _camera.getRight());
    } else if (GetKeyState(GLFW_KEY_A) == GLFW_PRESS) {
        _camera.move(deltaTime * moveSpeed * -_camera.getRight());
    }
}

void CWindowTest::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static const bool useLight = true;

    // Set shader program as the active one
    CProgram &current = useLight ? _shading : _simple;

    current.use();
    {
        // Model matrix: Scale * Rotation * Translation
        glm::mat4 modelMatrix = glm::rotate( glm::mat4(1.0f), glm::radians(_rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f) );
        glUniformMatrix4fv(current.getUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

        _light.setPosition(_camera.getPosition());
        _light.setDirection(_camera.getDirection());

        _model.render();
    }
    current.unuse();

    SetCursorPosition(_width/2.0f, _height/2.0f);

    utils::checkErrorStatus();
}

void CWindowTest::Resize(int width, int height)
{
    utils::Log(utils::CFormat(L"CWindowTest::Resize(%%x%%)") << width << height, utils::ELogLevel::Debug);

    _camera.setAspectRatio( static_cast<float>(_width / _height) );

    glViewport(0, 0, width, height);
}

void CWindowTest::Key(int key, int action, int mods)
{
    UNUSED(mods);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        Destroy();
    } else if (key == GLFW_KEY_LEFT) {
        if (action == GLFW_PRESS) {
            _rotateSpeed = 45.0f;
        } else if (action == GLFW_RELEASE && _rotateSpeed > 0.0f) {
            _rotateSpeed = 0.0f;
        }
    } else if (key == GLFW_KEY_RIGHT) {
        if (action == GLFW_PRESS) {
            _rotateSpeed = -45.0f;
        } else if (action == GLFW_RELEASE && _rotateSpeed < 0.0f) {
            _rotateSpeed = 0.0f;
        }
    }
}

void CWindowTest::Cursor(float xpos, float ypos)
{
    // utils::Log(utils::CFormat(L"CWindowTest::Cursor(%%, %%)") << xpos << ypos, utils::ELogLevel::Debug);

    static const float mouseSensitivity = 0.0005f;

    const float rotate_x = _height/2.0f - ypos;
    const float rotate_y = _width/2.0f - xpos;

    _camera.pitch(rotate_x * mouseSensitivity);
    _camera.rotate(rotate_y * mouseSensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
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
