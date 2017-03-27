#include <Version.h>
#include <CWindowManager.h>
#include <CLogger.h>
#include <CMesh.h>
#include <CCamera.h>

class CWindowTest : public CWindow
{
public:
    CWindowTest(int width, int height, const std::wstring &title)
        : CWindow(width, height, title)
        , _angle(0.0f)
        , _speed(45.0f)
        , _movementDirection(EDirection::No)
    {
        utils::Log(utils::CFormat(L"CWindowTest::CWindowTest(%%)") << title, utils::ELogLevel::Debug);
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
    CCamera   _camera;
    CMesh     _cubeMesh;

    float     _angle;
    float     _speed;

    EDirection _movementDirection;
};

void CWindowTest::Init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

    // Scale * Rotation * Translation

    {
        glm::mat4 modelMatrix = glm::rotate( glm::mat4(1.0f), glm::radians(_angle), glm::vec3(0.0f, 0.0f, 1.0f) );
        _cubeMesh.Render(_camera.viewProjection() * modelMatrix);
    }

    {
        glm::mat4 rotationMatrix = glm::rotate( glm::mat4(1.0f), glm::radians(_angle + 135.0f), glm::vec3(0.0f, 0.0f, 1.0f) );
        glm::mat4 translationMatrix = glm::translate( glm::mat4(1.0f), glm::vec3(3.0f, 2.0f, 0.0f) );
        _cubeMesh.Render(_camera.viewProjection() * translationMatrix * rotationMatrix);
    }

    CenterMouse();

    utils::CheckOpenGLError();
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
    } else if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
            _movementDirection |= EDirection::Left;
        } else if (action == GLFW_RELEASE) {
            _movementDirection &= ~EDirection::Left;
        }
    } else if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            _movementDirection |= EDirection::Right;
        } else if (action == GLFW_RELEASE) {
            _movementDirection &= ~EDirection::Right;
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
        WindowManager().Create<CWindowTest>(640, 480, L"CoNE3D Window Test");
        WindowManager().Process();
    }
    catch (const std::exception &ex)
    {
        utils::Log(ex.what(), utils::ELogLevel::Error);
    }

    return 0;
}
