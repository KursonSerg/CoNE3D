#ifndef C_WINDOW_H
#define C_WINDOW_H

#include <Common.h>
#include <CLogger.h>

class CWindow
{
public:
    CWindow(int width, int height, const std::wstring &title);
    CWindow(CWindow &&other) = default;
    virtual ~CWindow()       = default;

    virtual void Init();
    virtual void Update(float deltaTime);
    virtual void Render();
    virtual void Resize(int width, int height);
    virtual void Key(int key, int action, int mods);
    virtual void Cursor(float xpos, float ypos);

    void Process();

    void Destroy()
    {
        glfwSetWindowShouldClose(_window.get(), 1);
    }

    bool operator==(const CWindow &other) const
    {
        return _window.get() == other._window.get();
    }

    bool operator!=(const CWindow &other) const
    {
        return !(*this == other);
    }

protected:
    bool Running()
    {
        return glfwWindowShouldClose( _window.get() ) != 1;
    }

    void CenterMouse()
    {
        glfwSetCursorPos(_window.get(), static_cast<double>(_width/2.0f), static_cast<double>(_height/2.0f));
    }

    float _width;
    float _height;

private:
    // Callbacks
    static void FramebufferSizeCallback(GLFWwindow *target_window, int width, int height);
    static void KeyCallback(GLFWwindow *target_window, int key, int scancode, int action, int mods);
    static void CursorPosCallback(GLFWwindow *target_window, double xpos, double ypos);

    class GLFWwindowDestroy
    {
    public:
        void operator()(GLFWwindow *window)
        {
            glfwDestroyWindow(window);
        }
    };

    std::unique_ptr<GLFWwindow, GLFWwindowDestroy> _window;
};

#endif // C_WINDOW_H
