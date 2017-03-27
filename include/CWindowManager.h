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

    std::unique_ptr<CWindow> _window;
};

inline CWindowManager &WindowManager()
{
    return CWindowManager::Instance();
}

template<typename CType>
CWindow *CWindowManager::Create(int width, int height, const std::wstring &title)
{
    _window.reset( new CType(width, height, title) );
    _window->Init();
    _window->Resize(width, height);

    return _window.get();
}

#endif // C_WINDOW_MANAGER_H
