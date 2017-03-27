#include <CFormat.h>

namespace utils {
CFormat::CFormat(const std::wstring &format_string)
{
    std::wstring::size_type pos = 0, last_pos = 0;

    while (std::wstring::npos != pos && std::wstring::npos != last_pos) {
        pos = format_string.find(L"%%", last_pos);

        // Found a token, add it to the list
        _tokens.push_back( format_string.substr(last_pos, pos - last_pos) );

        // Skip delimiters
        last_pos = pos + 2;
    }

    _formatString = _tokens.front();
    _tokens.pop_front();
}

CFormat &CFormat::Append(const std::wstring &arg)
{
    if ( !_tokens.empty() ) {
        _formatString += arg + _tokens.front();
        _tokens.pop_front();
    }

    return *this;
}
} // namespace utils
