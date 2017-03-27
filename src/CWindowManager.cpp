#include <CWindowManager.h>

CWindowManager::CWindowManager()
{
    utils::Log(utils::CFormat(L"CWindowManager::CWindowManager()"), utils::ELogLevel::Debug);

    // Initialize glfw library
    if ( !glfwInit() ) {
        throw std::runtime_error("Failed to initialize window library");
    }
}

CWindowManager::~CWindowManager()
{
    utils::Log(utils::CFormat(L"CWindowManager::~CWindowManager()"), utils::ELogLevel::Debug);

    // Clean up glfw library
    glfwTerminate();
}

void CWindowManager::Process()
{
    if ( _window ) {
        // Loop until the user closes main window
        _window->Process();
        _window->Destroy();
        _window.reset();
    }
}
