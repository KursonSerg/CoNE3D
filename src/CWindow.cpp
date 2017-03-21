#include <CWindow.h>

const double MAX_FPS         = 1000.0 / 120.0;
const double TIME_PER_UPDATE = MAX_FPS / 1000.0;

CWindow::CWindow(int width, int height, const std::wstring &title) :
    m_last_update_time(0.0),
    m_delta_time(0.0)
{
    utils::Log(utils::CFormat(L"CWindow::CWindow(%%)") << title, utils::ELogLevel::Debug);

    // Set OpenGL 3.3 Core context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

    // Create a windowed mode window and its OpenGL context
    m_window.reset( glfwCreateWindow(width, height, utils::ws2s(title).c_str(), nullptr, nullptr) );
    if (!m_window) {
        throw std::runtime_error("Failed to create window with OpenGL context");
    }

    // Make the window's context current
    glfwMakeContextCurrent( m_window.get() );

    if ( !gladLoadGLLoader( reinterpret_cast<GLADloadproc>(glfwGetProcAddress) ) ) {
        throw std::runtime_error("Failed to initialize OpenGL context");
    }

    utils::Log(utils::CFormat(L"OpenGL %%, GLSL %%") << glGetString(GL_VERSION)
               << glGetString(GL_SHADING_LANGUAGE_VERSION), utils::ELogLevel::Info);
    utils::Log(utils::CFormat(L"OpenGL version %%.%% loaded")
               << GLVersion.major << GLVersion.minor, utils::ELogLevel::Debug);

#ifdef _DEBUG
    if (glDebugMessageCallback) {
        utils::Log(L"Registering OpenGL debug message callback...", utils::ELogLevel::Debug);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(utils::OpenGLDebugMessageCallback, nullptr);
        GLuint ids = 0;
        glDebugMessageControl(GL_DONT_CARE,
                              GL_DONT_CARE,
                              GL_DONT_CARE,
                              0,
                              &ids,
                              true);
    } else {
        utils::Log(L"glDebugMessageCallback is not available", utils::ELogLevel::Warning);
    }
#endif

    m_width = width;
    m_height = height;

    glfwSetWindowUserPointer(m_window.get(), this);
    glfwSetFramebufferSizeCallback(m_window.get(), FramebufferSizeCallback);
    glfwSetKeyCallback(m_window.get(), KeyCallback);
    // glfwSetInputMode(m_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(m_window.get(), CursorPosCallback);
    // glfwSetInputMode(m_window.get(), GLFW_STICKY_KEYS, 1);
}

void CWindow::Process()
{
    while ( !ShouldClose() ) {
        if (m_last_update_time == 0.0) {
            m_last_update_time = glfwGetTime();
        }

        double current = glfwGetTime();
        double elapsed = current - m_last_update_time;
        m_last_update_time = current;
        m_delta_time      += elapsed;

        while (m_delta_time >= TIME_PER_UPDATE) {
            Update(TIME_PER_UPDATE);
            m_delta_time -= TIME_PER_UPDATE;
        }

        // Render window
        Render();

        // Swap front and back buffers
        glfwSwapBuffers( m_window.get() );
        // Poll for and process events
        glfwPollEvents();
    }
}

void CWindow::FramebufferSizeCallback(GLFWwindow *target_window, int width, int height)
{
    CWindow *window = static_cast<CWindow *>( glfwGetWindowUserPointer(target_window) );
    window->m_width = width;
    window->m_height = height;
    window->Resize(width, height);
}

void CWindow::KeyCallback(GLFWwindow *target_window, int key, int scancode, int action, int mods)
{
    CWindow *window = static_cast<CWindow *>( glfwGetWindowUserPointer(target_window) );
    window->Key(key, scancode, action, mods);
}

void CWindow::CursorPosCallback(GLFWwindow *target_window, double xpos, double ypos)
{
    CWindow *window = static_cast<CWindow *>( glfwGetWindowUserPointer(target_window) );
    window->Cursor(xpos, ypos);
}

void CWindow::Init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void CWindow::Update(double delta)
{
    UNUSED(delta);
}

void CWindow::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CWindow::Resize(int width, int height)
{
    glViewport(0, 0, width, height);
}

void CWindow::Key(int key, int scancode, int action, int mods)
{
    UNUSED(key);
    UNUSED(scancode);
    UNUSED(action);
    UNUSED(mods);
}

void CWindow::Cursor(double xpos, double ypos)
{
    UNUSED(xpos);
    UNUSED(ypos);
}
