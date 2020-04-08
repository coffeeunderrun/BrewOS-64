#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <sys/types.h>

namespace BrewOS {

template <class T>
class Vector {
private:
    T *m_data;

    unsigned int m_limit;

    unsigned int m_count;

    bool Expand(void)
    {
        unsigned int new_limit = m_limit * 2;
        T *data = (T *)realloc(m_data, sizeof(T) * new_limit);
        if(data == nullptr)
        {
            // Not enough memory
            return false;
        }

        m_data = data;
        m_limit = new_limit;

        return true;
    }

    void Shrink(void)
    {
        m_limit /= 2;
        m_data = (T *)realloc(m_data, sizeof(T) * m_limit);
    }

public:
    Vector(unsigned int limit = 10)
    {
        if(limit == 0)
        {
            limit = 1;
        }

        m_data = new T[limit];
        m_limit = limit;
        m_count = 0;
    }

    ~Vector(void)
    {
        delete m_data;
    }

    void Push(T item)
    {
        if(m_count == m_limit && !Expand())
        {
            // Not enough memory
            return;
        }

        m_data[m_count++] = item;
    }

    T Pop(void)
    {
        if(m_count == 0)
        {
            // No items left
            return 0;
        }

        if(m_count == m_limit / 4)
        {
            Shrink();
        }

        return m_data[--m_count];
    }

    void Insert(T item, unsigned int index)
    {
        if(index >= m_count)
        {
            // Out of bounds
            return;
        }

        if(m_count == m_limit && !Expand())
        {
            // Not enough memory
            return;
        }

        unsigned int move_size = (m_count - index) * sizeof(T);
        memmove(&m_data[index + 1], &m_data[index], move_size);

        m_data[index] = item;
        m_count++;
    }

    T Remove(unsigned int index)
    {
        if(index >= m_count)
        {
            // Out of bounds
            return;
        }

        if(m_count == m_limit / 4)
        {
            Shrink();
        }

        T item = m_data[index];

        unsigned int move_size = (m_count - index) * sizeof(T);
        memmove(&m_data[index], &m_data[index + 1], move_size);

        m_count--;

        return item;
    }

    void Set(T item, unsigned int index)
    {
        if(index >= m_count)
        {
            // Out of bounds
            return;
        }

        m_data[index] = item;
    }

    T Get(unsigned int index)
    {
        if(index >= m_count)
        {
            // Out of bounds
            return;
        }

        return m_data[index];
    }

    unsigned int GetLimit(void)
    {
        return m_limit;
    }

    unsigned int GetCount(void)
    {
        return m_count;
    }
};

} // BrewOS

#endif // VECTOR_H
