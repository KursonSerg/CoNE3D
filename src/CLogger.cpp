#include <CLogger.h>

namespace utils {
#if LOG_TO_FILE
static constexpr const char *s_log_filename = "CoNE3D.log";
#else
static constexpr const char *s_log_filename = "";
#endif

CLogger::CLogger() noexcept :
    m_log(s_log_filename)
{
    std::ios_base::sync_with_stdio(false);
    getStream().imbue( std::locale(std::locale(), new std::codecvt_utf8<wchar_t>) );
}

CLogger::~CLogger() noexcept
{
    m_log.close();
}

std::wostream &CLogger::getStream() noexcept
{
    if ( m_log.is_open() ) {
        return m_log;
    }

    return std::wclog;
}

void Log(const std::wstring &message, ELogLevel log_level)
{
    if (log_level <= LOG_LEVEL) {
        std::lock_guard<std::mutex> guard(CLogger::Log().m_log_sync);

        std::wostream &log = CLogger::Log().getStream();

        switch (log_level) {
        case ELogLevel::Error:
            log << L"[ERROR] ";
            break;
        case ELogLevel::Warning:
            log << L"[WARNING] ";
            break;
        case ELogLevel::Info:
            log << L"[INFO] ";
            break;
        case ELogLevel::Debug:
            log << L"[DEBUG] ";
            break;
        default:
            break;
        }

        log << message << std::endl;
        log.flush();
        log.clear();
    }
}
} // namespace utils
