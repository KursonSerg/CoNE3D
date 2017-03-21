#include <thread>

//#include <gli/texture.hpp>
//#include <gli/load.hpp>
//#include <gli/gl.hpp>

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
        , m_angle(0.0)
        , m_speed(0.0)
        , m_dir(EDirection::No)
    {
        utils::Log(utils::CFormat(L"CWindowTest::CWindowTest(%%)") << title, utils::ELogLevel::Debug);
    }

    virtual ~CWindowTest()
    {
        utils::Log(utils::CFormat(L"CWindowTest::~CWindowTest()"), utils::ELogLevel::Debug);
    }

    virtual void Init() override;
    virtual void Update(double delta) override;
    virtual void Render() override;
    virtual void Resize(int width, int height) override;
    virtual void Key(int m_dir, int scancode, int action, int mods) override;
    virtual void Cursor(double xpos, double ypos) override;

private:
    CCamera   m_camera;
    CMesh     m_mesh;

    glm::mat4 m_projection_matrix;

    float     m_angle;
    double    m_speed;

    EDirection m_dir;
};

void CWindowTest::Init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}

void CWindowTest::Update(double delta)
{
    m_angle += m_speed * delta;
    if (m_angle >= 360.0f) {
        m_angle = 0.0f;
    }

    float deltaTime = 0.1f;
    float speed = 1.0f;

    EDirection a;
//    (int &)a |= 1;
    a |= EDirection::Backward;

    // Двигаемся вперед
    if (has_flag(m_dir & EDirection::Forward)) {
       m_camera.m_position += m_camera.m_direction * deltaTime * speed;
    }
    // Двигаемся назад
    if (has_flag(m_dir & EDirection::Backward)) {
       m_camera.m_position -= m_camera.m_direction * deltaTime * speed;
    }
    // Шаг влево
    if (has_flag(m_dir & EDirection::Left)) {
       m_camera.m_position -= m_camera.m_right * deltaTime * speed;
    }
    // Шаг вправо
    if (has_flag(m_dir & EDirection::Right)) {
       m_camera.m_position += m_camera.m_right * deltaTime * speed;
    }

    m_camera.update();
}

void CWindowTest::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Scale * Rotation * Translation

    // Create MVP matrix
    glm::mat4 model_matrix = glm::rotate( glm::mat4(1.0f), m_angle, glm::vec3(0.0f, 0.0f, 1.0f) );
//    glm::mat4 view_matrix = glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f) );

    glm::mat4 mvp_matrix = m_projection_matrix * m_camera.GetViewMatrix() * model_matrix;
    m_mesh.Render(mvp_matrix);

    CenterMouse();

    utils::CheckOpenGLError();
}

void CWindowTest::Resize(int width, int height)
{
    utils::Log(utils::CFormat(L"CWindowTest::Resize(%%x%%)") << width << height, utils::ELogLevel::Debug);

    float aspect = static_cast<float>(m_width / m_height);
    m_projection_matrix = glm::perspective( glm::radians(45.0f), aspect, 0.1f, 100.0f );

    // Set viewport
    glViewport(0, 0, width, height);
}

void CWindowTest::Key(int key, int scancode, int action, int mods)
{
    UNUSED(scancode);
    UNUSED(mods);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        Destroy();
    }/* else if (key == GLFW_KEY_LEFT) {
        if (action == GLFW_PRESS) {
            m_speed = 1.0;
        } else if (action == GLFW_RELEASE && m_speed > 0.0) {
            m_speed = 0.0;
        }
    } else if (key == GLFW_KEY_RIGHT) {
        if (action == GLFW_PRESS) {
            m_speed = -1.0;
        } else if (action == GLFW_RELEASE && m_speed < 0.0) {
            m_speed = 0.0;
        }
    }*/

//    utils::Log(utils::CFormat(L"CWindowTest::Key(%%x%%)") << key << action, utils::ELogLevel::Debug);

    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) {
            m_dir = EDirection(m_dir | EDirection::Forward);
        } else if (action == GLFW_RELEASE) {
            m_dir = EDirection(m_dir & ~EDirection::Forward);
        }
    } else if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            m_dir = EDirection(m_dir | EDirection::Backward);
        } else if (action == GLFW_RELEASE) {
            m_dir = EDirection(m_dir & ~EDirection::Backward);
        }
    } else if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
            m_dir = EDirection(m_dir | EDirection::Left);
        } else if (action == GLFW_RELEASE) {
            m_dir = EDirection(m_dir & ~EDirection::Left);
        }
    } else if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            m_dir = EDirection(m_dir | EDirection::Right);
        } else if (action == GLFW_RELEASE) {
            m_dir = EDirection(m_dir & ~EDirection::Right);
        }
    }
}

void CWindowTest::Cursor(double xpos, double ypos)
{
//    utils::Log(utils::CFormat(L"CWindowTest::Cursor(%%x%%)") << xpos << ypos, utils::ELogLevel::Debug);

    m_camera.LookAt( float(m_width/2.0 - xpos), float(m_height/2.0 - ypos) );
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
