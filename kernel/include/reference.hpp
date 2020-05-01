#ifndef KERNEL_REFERENCE
#define KERNEL_REFERENCE

#include <cassert>

namespace BrewOS {

class Reference {
  private:
    int m_cnt;

  public:
    Reference(int cnt = 0)
      : m_cnt(cnt)
    {
        assert(m_cnt >= 0);
    }

    ~Reference(void)
    {
        assert(m_cnt == 0);
    }

    Reference& operator=(const Reference&)
    {
        return *this;
    }

    bool operator==(const int val)
    {
        return m_cnt == val;
    }

    bool operator!=(const int val)
    {
        return m_cnt != val;
    }

    bool operator>(const int val)
    {
        return m_cnt > val;
    }

    bool operator>=(const int val)
    {
        return m_cnt >= val;
    }

    bool operator<(const int val)
    {
        return m_cnt < val;
    }

    bool operator<=(const int val)
    {
        return m_cnt <= val;
    }

    int& operator--(void)
    {
        assert(m_cnt > 0);
        return --m_cnt;
    }

    int operator--(int)
    {
        assert(m_cnt > 0);
        return m_cnt--;
    }

    int& operator++(void)
    {
        assert(m_cnt >= 0);
        return ++m_cnt;
    }

    int operator++(int)
    {
        assert(m_cnt >= 0);
        return m_cnt++;
    }
};

} // BrewOS

#endif // KERNEL_REFERENCE
