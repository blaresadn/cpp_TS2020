#ifndef HW7_ALLOCATOR_H
#define HW7_ALLOCATOR_H

#endif //HW7_ALLOCATOR_H

template <typename T>
struct Allocator
{
    typedef T value_type;

    T* allocate(size_t size) const
    {
        return new T[size];
    }

    void deallocate(T *p) const noexcept
    {
        delete[] p;
    }
};

template <typename T1, typename T2>
bool operator==(const Allocator<T1>&, const Allocator<T2>&)
{
    return true;
}

template <typename T1, typename T2>
bool operator!=(const Allocator<T1>&, const Allocator<T2>&)
{
    return false;
}
