#ifndef KERNEL_POINTER
#define KERNEL_POINTER

#include <cassert>
#include <reference.hpp>

namespace BrewOS {

template<class T>
class Pointer {
  private:
    T* m_data;

    Reference* m_ref;

  public:
    Pointer(void)
      : m_data(nullptr)
      , m_ref(new Reference())
    {
    }

    Pointer(T* data)
      : m_data(data)
      , m_ref(new Reference())
    {
        // Do not increase reference count if data is NULL
        if(m_data == nullptr) {
            return;
        }

        // Increase reference count
        (*m_ref)++;
    }

    Pointer(const Pointer<T>& ptr)
      : m_data(ptr.m_data)
      , m_ref(ptr.m_ref)
    {
        assert(m_ref != nullptr);

        // Do not increase reference count if data is NULL
        if(m_data == nullptr) {
            return;
        }

        // Increase reference count
        (*m_ref)++;
    }

    ~Pointer(void)
    {
        assert(m_ref != nullptr);

        // Do not delete data if there are still references
        if(--(*m_ref) > 0) {
            return;
        }

        if(m_data != nullptr) {
            delete m_data;
        }

        delete m_ref;
    }

    Pointer& operator=(const Pointer ptr)
    {
        assert(m_ref != nullptr);

        // Check if both pointers reference the same data
        if(m_data == ptr.m_data) {
            return *this;
        }

        // Remove reference to original data
        if(m_data != nullptr) {
            m_data = nullptr;
            (*m_ref)--;
        }

        // Use reference counter from new pointer
        m_ref = ptr.m_ref;

        // Use data from new pointer if available
        if(ptr.m_data != nullptr) {
            m_data = ptr.m_data;
            (*m_ref)++;
        }

        return *this;
    }

    T& operator*(void)
    {
        return *m_data;
    }

    T& operator[](int idx)
    {
        return m_data[idx];
    }

    T* operator->(void)
    {
        return m_data;
    }

    T* Get(void)
    {
        return m_data;
    }

    void Reset(T* data = nullptr)
    {
        assert(m_ref != nullptr);

        // Do not reset if original data is the same as the new data
        if(m_data == data) {
            return;
        }

        // Remove reference to original data
        if(m_data != nullptr) {
            (*m_ref)--;
        }

        // Create new reference counter if original is still being used
        if(*m_ref > 0) {
            m_ref = new Reference();
        }

        // Reference new data
        m_data = data;
        (*m_ref)++;
    }
};

} // BrewOS

#endif // KERNEL_POINTER
