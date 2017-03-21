#ifndef C_FORMAT_H
#define C_FORMAT_H

#include <locale>
#include <codecvt>

#include <Common.h>

namespace utils {
inline std::wstring s2ws(const std::string &str)
{
    return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
}

inline std::string ws2s(const std::wstring &wstr)
{
    return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr);
}

class CFormat
{
public:
    CFormat(const std::wstring &format_string);

    operator const std::wstring & () const
    {
        return m_format_string;
    }

    template<typename CType>
    CFormat &operator<<(const CType &val)
    {
        return Arg(val);
    }

private:
    CFormat &Arg(const wchar_t *str)
    {
        return Append( std::wstring(str) );
    }

    CFormat &Arg(const std::wstring &str)
    {
        return Append(str);
    }

    CFormat &Arg(const char *str)
    {
        return Append( s2ws(str) );
    }

    CFormat &Arg(const unsigned char *str)
    {
        return Append( s2ws( reinterpret_cast<const char *>(str) ) );
    }

    CFormat &Arg(const std::string &str)
    {
        return Append( s2ws(str) );
    }

    template<typename CType>
    CFormat &Arg(const CType &val)
    {
        return Append( std::to_wstring(val) );
    }

    CFormat &Append(const std::wstring &arg);

    std::list<std::wstring> m_tokens;
    std::wstring            m_format_string;
};
} // namespace utils

#endif // C_FORMAT_H
