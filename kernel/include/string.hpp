#ifndef KERNEL_STRING
#define KERNEL_STRING

#include <cstdint>

namespace BrewOS {

class String
{
private:
    char *m_str = nullptr;

    size_t m_cap = 0;
    size_t m_len = 0;

public:
    static const size_t npos = -1;

    String(size_t cap = 10);
    String(const char chr, size_t cnt);
    String(const char *str);
    String(const String &str);
    String(const String &str, size_t spos, size_t slen = npos);

    ~String(void);

    String &operator+=(const char chr) { return Append(chr, 1); }
    String &operator+=(const char *str) { return Append(str); }
    String &operator+=(const String &str) { return Append(str); }

    String &operator=(const char chr) { return Assign(chr, 1); }
    String &operator=(const char *str) { return Assign(str); }
    String &operator=(const String &str) { return Assign(str); }

    bool operator==(const char *str) { return Compare(str) == 0; }
    bool operator==(const String &str) { return Compare(str) == 0; }

    bool operator!=(const char *str) { return Compare(str) != 0; }
    bool operator!=(const String &str) { return Compare(str) != 0; }

    char &operator[](size_t idx) { return m_str[idx]; }

    char *begin(void) { return m_str; }
    char *end(void) { return (char *)(m_str + m_len); }

    String &Append(const char chr, size_t cnt);
    String &Append(const char *str);
    String &Append(const char *str, size_t len);
    String &Append(const String &str);
    String &Append(const String &str, size_t spos, size_t slen = npos);

    String &Assign(const char chr, size_t cnt);
    String &Assign(const char *str);
    String &Assign(const char *str, size_t len);
    String &Assign(const String &str);
    String &Assign(const String &str, size_t spos, size_t slen = npos);

    String &Insert(size_t pos, const char chr, size_t cnt);
    String &Insert(size_t pos, const char *str);
    String &Insert(size_t pos, const char *str, size_t slen);
    String &Insert(size_t pos, const String &str);
    String &Insert(size_t pos, const String &str, size_t spos, size_t slen = npos);

    int Compare(const char *str) const;
    int Compare(const String &str) const;
    int Compare(size_t pos, size_t len, const char *str) const;
    int Compare(size_t pos, size_t len, const char *str, size_t slen) const;
    int Compare(size_t pos, size_t len, const String &str) const;
    int Compare(size_t pos, size_t len, const String &str, size_t spos, size_t slen = npos) const;

    void Clear(void);

    bool Empty(void) const { return !m_len; }

    size_t Length(void) const { return m_len; }

private:
    void expand(size_t cap);

    void shrink(size_t cap);
};

String operator+(const char a, const String &b);
String operator+(const char *a, const String &b);
String operator+(const String &a, const char b);
String operator+(const String &a, const char *b);
String operator+(const String &a, const String &b);

}

#endif // KERNEL_STRING
