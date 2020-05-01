#ifndef KERNEL_VECTOR
#define KERNEL_VECTOR

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <types.hpp>

namespace BrewOS {

template<class T>
class Vector {
  private:
    T* m_data = nullptr;

    size_t m_cap = 0;
    size_t m_cnt = 0;

  public:
    Vector(size_t cap = 1)
      : m_cap(MAX(cap, 1))
    {
        m_data = (T*)malloc((m_cap) * sizeof(T));

        if(m_data == nullptr) {
            // PANIC
            return;
        }

        for(size_t i = 0; i < m_cap; i++) {
            m_data[i] = T{};
        }
    }

    ~Vector(void)
    {
        free(m_data);
    }

    void Push(T item)
    {
        if(m_cnt == m_cap && !Expand()) {
            // Not enough memory
            return;
        }

        m_data[m_cnt++] = item;
    }

    T Pop(void)
    {
        if(m_cnt == 0) {
            // No items left
            return 0;
        }

        if(m_cnt <= m_cap / 4) {
            Shrink();
        }

        return m_data[--m_cnt];
    }

    void Insert(T item, size_t index)
    {
        if(index >= m_cnt) {
            // Out of bounds
            return;
        }

        if(m_cnt == m_cap && !Expand()) {
            // Not enough memory
            return;
        }

        size_t move_size = (m_cnt - index) * sizeof(T);
        memmove(&m_data[index + 1], &m_data[index], move_size);

        m_data[index] = item;
        m_cnt++;
    }

    T Remove(size_t index)
    {
        if(index >= m_cnt) {
            // Out of bounds
            return T{};
        }

        if(m_cnt <= m_cap / 4) {
            Shrink();
        }

        T item = m_data[index];

        size_t move_size = (m_cnt - index) * sizeof(T);
        memmove(&m_data[index], &m_data[index + 1], move_size);

        m_cnt--;

        return item;
    }

    void Set(T item, size_t index)
    {
        if(index >= m_cnt) {
            // Out of bounds
            return T{};
        }

        m_data[index] = item;
    }

    T Get(size_t index)
    {
        if(index >= m_cnt) {
            // Out of bounds
            return T{};
        }

        return m_data[index];
    }

    unsigned Capacity(void)
    {
        return m_cap;
    }

    unsigned Count(void)
    {
        return m_cnt;
    }

    T* begin(void)
    {
        return &m_data[0];
    }

    T* end(void)
    {
        return (T*)(m_data + m_cnt);
    }

  private:
    bool Expand(void)
    {
        size_t new_cap = m_cap * 2;

        T* data = (T*)realloc(m_data, sizeof(T) * new_cap);

        if(data == nullptr) {
            // Not enough memory
            return false;
        }

        m_data = data;
        m_cap = new_cap;

        for(size_t i = m_cnt; i < m_cap; i++) {
            m_data[i] = T{};
        }

        return true;
    }

    void Shrink(void)
    {
        m_cap = m_cap / 2;
        m_data = (T*)realloc(m_data, sizeof(T) * m_cap);

        assert(m_data != nullptr);
    }
};

} // BrewOS

#endif // KERNEL_VECTOR
