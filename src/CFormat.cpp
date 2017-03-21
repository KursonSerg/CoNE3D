#include <CFormat.h>

namespace utils {
CFormat::CFormat(const std::wstring &format_string)
{
    std::wstring::size_type pos = 0, last_pos = 0;

    while (std::wstring::npos != pos && std::wstring::npos != last_pos) {
        pos = format_string.find(L"%%", last_pos);

        // Found a token, add it to the list
        m_tokens.push_back( format_string.substr(last_pos, pos - last_pos) );

        // Skip delimiters
        last_pos = pos + 2;
    }

    m_format_string = m_tokens.front();
    m_tokens.pop_front();
}

CFormat &CFormat::Append(const std::wstring &arg)
{
    if ( !m_tokens.empty() ) {
        m_format_string += arg + m_tokens.front();
        m_tokens.pop_front();
    }

    return *this;
}
} // namespace utils
