#include <cassert>
#include <cstdlib>
#include <cstring>
#include <string.hpp>
#include <types.hpp>

namespace BrewOS {

String::String(size_t cap)
  : m_cap(cap)
{
    m_str = (char*)malloc((m_cap + 1) * sizeof(char));

    if(m_str == nullptr) {
        // PANIC
    }
}

String::String(const char chr, size_t cnt)
  : String(cnt)
{
    m_str = (char*)memchr(m_str, chr, cnt);
    m_len = cnt;
}

String::String(const char* str)
  : String(strlen(str))
{
    strcpy(m_str, str);
    m_len = m_cap;
}

String::String(const String& str)
  : String(str.m_str)
{
}

String::String(const String& str, size_t pos, size_t len)
{
    m_cap = m_len = MIN(len, str.m_len) - pos;
    m_str = (char*)malloc((m_cap + 1) * sizeof(char));

    if(m_str == nullptr) {
        // PANIC
        return;
    }

    strncpy(m_str, (char*)(str.m_str + pos), m_len);
}

String::~String(void)
{
    free(m_str);
}

String&
String::Append(const char* str)
{
    Expand(m_len + strlen(str));

    strncat(m_str, str, m_cap);
    m_len = strlen(m_str);

    return *this;
}

String&
String::Assign(const char* str)
{
    Expand(strlen(str));

    strncpy(m_str, str, m_cap);
    m_len = strlen(m_str);

    return *this;
}

void
String::Clear(void)
{
    m_str[0] = '\0';
    m_len = 0;
}

void
String::Expand(size_t cap)
{
    if(cap <= m_cap) {
        return;
    }

    char* str = (char*)realloc(m_str, (cap + 1) * sizeof(char));

    if(str == nullptr) {
        // PANIC
        return;
    }

    m_str = str;
    m_cap = cap;
}

void
String::Shrink(size_t cap)
{
    if(cap >= m_cap) {
        return;
    }

    char* str = (char*)realloc(m_str, (cap + 1) * sizeof(char));

    if(str == nullptr) {
        // PANIC
        return;
    }

    m_str = str;
    m_cap = m_len = cap;
}

} // BrewOS
