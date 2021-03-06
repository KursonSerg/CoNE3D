#include <Window.h>

CWindow::CWindow(int width, int height, const std::wstring &title)
{
    utils::Log(utils::CFormat(L"CWindow::CWindow(%%)") << title, utils::ELogLevel::Debug);

    // Set OpenGL 3.3 Core context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

    // Create a windowed mode window and its OpenGL context
    _window.reset( glfwCreateWindow(width, height, utils::ws2s(title).c_str(), nullptr, nullptr) );
    if (!_window) {
        throw std::runtime_error("Failed to create window with OpenGL context");
    }

    // Make the window's context current
    glfwMakeContextCurrent( _window.get() );

    if ( !gladLoadGLLoader( reinterpret_cast<GLADloadproc>(glfwGetProcAddress) ) ) {
        throw std::runtime_error("Failed to initialize OpenGL context");
    }

    utils::Log(utils::CFormat(L"OpenGL %%, GLSL %%") << glGetString(GL_VERSION)
               << glGetString(GL_SHADING_LANGUAGE_VERSION), utils::ELogLevel::Info);
    utils::Log(utils::CFormat(L"OpenGL version %%.%% loaded")
               << GLVersion.major << GLVersion.minor, utils::ELogLevel::Debug);

#ifndef NDEBUG
    if (glDebugMessageCallback) {
        utils::Log(L"Registering OpenGL debug message callback...", utils::ELogLevel::Debug);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(utils::debugMessageCallback, nullptr);
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

    _width = static_cast<float>(width);
    _height = static_cast<float>(height);

    glfwSetWindowUserPointer(_window.get(), this);
    glfwSetFramebufferSizeCallback(_window.get(), FramebufferSizeCallback);
    glfwSetKeyCallback(_window.get(), KeyCallback);
#if defined(NDEBUG) || defined(_WIN32) || defined(WIN32)
    glfwSetInputMode(_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif
    glfwSetCursorPosCallback(_window.get(), CursorPosCallback);
    glfwSetInputMode(_window.get(), GLFW_STICKY_KEYS, 1);
}

void CWindow::Process()
{
    glfwSwapInterval(0);

#ifndef NDEBUG
    int nbFrames = 0;
    double elapsedTime = 0.0;
#endif

    double previousTime = glfwGetTime();
    while ( Running() ) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;
        previousTime = currentTime;

#ifndef NDEBUG
        elapsedTime += deltaTime; ++nbFrames;
        if (elapsedTime >= 1.0) {
            utils::Log(utils::CFormat(L"Performance %% ms/frame")
                       << 1000.0 / static_cast<double>(nbFrames), utils::ELogLevel::Debug);
            elapsedTime -= 1.0; nbFrames = 0;
        }
#endif

        // Poll for and process events
        glfwPollEvents();

        Update(static_cast<float>(deltaTime));

        // Render window
        Render();

        // Swap front and back buffers
        glfwSwapBuffers( _window.get() );
    }

    Destroy();
}

void CWindow::FramebufferSizeCallback(GLFWwindow *target_window, int width, int height)
{
    CWindow *window = static_cast<CWindow *>( glfwGetWindowUserPointer(target_window) );

    window->_width = static_cast<float>(width);
    window->_height = static_cast<float>(height);

    window->Resize(width, height);
}

void CWindow::KeyCallback(GLFWwindow *target_window, int key, int scancode, int action, int mods)
{
    UNUSED(scancode);

    CWindow *window = static_cast<CWindow *>( glfwGetWindowUserPointer(target_window) );
    window->Key(key, action, mods);
}

void CWindow::CursorPosCallback(GLFWwindow *target_window, double xpos, double ypos)
{
    CWindow *window = static_cast<CWindow *>( glfwGetWindowUserPointer(target_window) );
    window->Cursor(static_cast<float>(xpos), static_cast<float>(ypos));
}

void CWindow::Init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void CWindow::Update(float deltaTime)
{
    UNUSED(deltaTime);
}

void CWindow::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CWindow::Resize(int width, int height)
{
    glViewport(0, 0, width, height);
}

void CWindow::Key(int key, int action, int mods)
{
    UNUSED(key);
    UNUSED(action);
    UNUSED(mods);
}

void CWindow::Cursor(float xpos, float ypos)
{
    UNUSED(xpos);
    UNUSED(ypos);
}
