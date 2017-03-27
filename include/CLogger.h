#ifndef C_LOGGER_H
#define C_LOGGER_H

#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>

#include <CFormat.h>

namespace utils {
enum class ELogLevel {
    Error,
    Warning,
    Info,
    Debug
};

#define LOG_LEVEL ELogLevel::Debug
#define LOG_TO_FILE 0

class CLogger
{
public:
    static CLogger &Log() noexcept
    {
        static CLogger logger;
        return logger;
    }

private:
    std::wofstream _log;
    std::mutex     _logSync;

    CLogger() noexcept;
    ~CLogger() noexcept;

    CLogger(const CLogger &)            = delete;
    CLogger &operator=(const CLogger &) = delete;

    std::wostream &getStream() noexcept;

    friend void Log(const std::wstring &format_string, ELogLevel log_level);
};

void Log(const std::wstring &format_string, ELogLevel log_level = ELogLevel::Info);

inline void Log(const std::string &message, ELogLevel log_level = ELogLevel::Info)
{
    Log(s2ws(message), log_level);
}

inline void Log(const CFormat &format, ELogLevel log_level = ELogLevel::Info)
{
    Log(static_cast<std::wstring>(format), log_level);
}
} // namespace utils

#endif // C_LOGGER_H
