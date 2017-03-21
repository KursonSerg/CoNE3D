#ifndef C_WINDOW_MANAGER_H
#define C_WINDOW_MANAGER_H

#include <CWindow.h>

class CWindowManager
{
public:
    static CWindowManager &Instance()
    {
        static CWindowManager manager;
        return manager;
    }

    template<typename CType>
    CWindow *Create(int width, int height, const std::wstring &title);
    void Process();

private:
    CWindowManager();
    ~CWindowManager();

    CWindowManager(const CWindowManager &)            = delete;
    CWindowManager &operator=(const CWindowManager &) = delete;

    std::unique_ptr<CWindow> m_window;
};

inline CWindowManager &WindowManager()
{
    return CWindowManager::Instance();
}

template<typename CType>
CWindow *CWindowManager::Create(int width, int height, const std::wstring &title)
{
    m_window.reset( new CType(width, height, title) );
    m_window->Init();
    m_window->Resize(width, height);

    return m_window.get();
}

#endif // C_WINDOW_MANAGER_H
