#ifndef C_WINDOW_H
#define C_WINDOW_H

#include <Common.h>
#include <CLogger.h>

class CWindow
{
public:
    CWindow(CWindow &&other) = default;
    virtual ~CWindow()       = default;

    virtual void Init();
    virtual void Update(double delta);
    virtual void Render();
    virtual void Resize(int width, int height);
    virtual void Key(int key, int scancode, int action, int mods);
    virtual void Cursor(double xpos, double ypos);

    void Process();

    void Destroy()
    {
        glfwSetWindowShouldClose(m_window.get(), 1);
    }

    bool operator==(const CWindow &other) const
    {
        return m_window.get() == other.m_window.get();
    }

    bool operator!=(const CWindow &other) const
    {
        return !(*this == other);
    }

protected:
    CWindow(int width, int height, const std::wstring &title);

    bool ShouldClose()
    {
        return glfwWindowShouldClose( m_window.get() ) == 1;
    }

    void CenterMouse()
    {
        glfwSetCursorPos(m_window.get(), m_width/2.0, m_height/2.0);
    }

    double m_width;
    double m_height;

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

    std::unique_ptr<GLFWwindow, GLFWwindowDestroy> m_window;

    double m_last_update_time;
    double m_delta_time;
};

#endif // C_WINDOW_H
