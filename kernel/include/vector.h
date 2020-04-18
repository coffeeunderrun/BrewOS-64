#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

namespace BrewOS {

template <class T>
class Vector {
private:
    T *m_data;

    unsigned m_lim;

    unsigned m_cnt;

    bool Expand(void)
    {
        unsigned new_limit = m_lim * 2;
        T *data = (T *)realloc(m_data, sizeof(T) * new_limit);
        if(data == nullptr)
        {
            // Not enough memory
            return false;
        }

        m_data = data;
        m_lim = new_limit;

        for(unsigned i = m_cnt; i < m_lim; i++)
        {
            m_data[i] = T{};
        }

        return true;
    }

    void Shrink(void)
    {
        m_lim /= 2;
        m_data = (T *)realloc(m_data, sizeof(T) * m_lim);
    }

public:
    Vector(unsigned lim = 10)
    {
        if(lim == 0)
        {
            lim = 1;
        }

        m_data = new T[lim];
        m_lim = lim;
        m_cnt = 0;

        for(unsigned i = 0; i < m_lim; i++)
        {
            m_data[i] = T{};
        }
    }

    ~Vector(void)
    {
        delete m_data;
    }

    void Push(T item)
    {
        if(m_cnt == m_lim && !Expand())
        {
            // Not enough memory
            return;
        }

        m_data[m_cnt++] = item;
    }

    T Pop(void)
    {
        if(m_cnt == 0)
        {
            // No items left
            return 0;
        }

        if(m_cnt == m_lim / 4)
        {
            Shrink();
        }

        return m_data[--m_cnt];
    }

    void Insert(T item, unsigned index)
    {
        if(index >= m_cnt)
        {
            // Out of bounds
            return;
        }

        if(m_cnt == m_lim && !Expand())
        {
            // Not enough memory
            return;
        }

        unsigned move_size = (m_cnt - index) * sizeof(T);
        memmove(&m_data[index + 1], &m_data[index], move_size);

        m_data[index] = item;
        m_cnt++;
    }

    T Remove(unsigned index)
    {
        if(index >= m_cnt)
        {
            // Out of bounds
            return T{};
        }

        if(m_cnt == m_lim / 4)
        {
            Shrink();
        }

        T item = m_data[index];

        unsigned move_size = (m_cnt - index) * sizeof(T);
        memmove(&m_data[index], &m_data[index + 1], move_size);

        m_cnt--;

        return item;
    }

    void Set(T item, unsigned index)
    {
        if(index >= m_cnt)
        {
            // Out of bounds
            return T{};
        }

        m_data[index] = item;
    }

    T Get(unsigned index)
    {
        if(index >= m_cnt)
        {
            // Out of bounds
            return T{};
        }

        return m_data[index];
    }

    unsigned GetLimit(void)
    {
        return m_lim;
    }

    unsigned GetCount(void)
    {
        return m_cnt;
    }
};

} // BrewOS

#endif // VECTOR_H
