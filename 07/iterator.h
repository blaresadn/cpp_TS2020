#include <iterator>

#ifndef HW7_ITERATOR_H
#define HW7_ITERATOR_H

#endif //HW7_ITERATOR_H

template <typename T>
class Iterator {
private:
    T* ptr_;

public:
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef std::random_access_iterator_tag iterator_category;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;

    explicit Iterator(): ptr_(nullptr)
    {
    }

    explicit Iterator(T *p): ptr_(p)
    {
    }

    Iterator &operator +=(difference_type n)
    {
        ptr_ += n;
        return *this;
    }

    Iterator operator +(difference_type n) const
    {
        Iterator tmp(*this);
        return tmp += n;
    }

    friend Iterator operator +(difference_type n, const Iterator &it);

    Iterator &operator -=(difference_type n)
    {
        ptr_ -= n;
        return *this;
    }

    Iterator operator -(difference_type n) const
    {
        Iterator tmp(*this);
        return tmp -= n;
    }

    friend Iterator operator -(difference_type n, const Iterator &it);

    bool operator ==(const Iterator& rhs) const
    {
        return ptr_ == rhs.ptr_;
    }

    bool operator !=(const Iterator& rhs) const
    {
        return !(*this == rhs);
    }

    bool operator >(const Iterator& rhs) const
    {
        return ptr_ - rhs.ptr_ > 0;
    }

    bool operator >=(const Iterator& rhs) const
    {
        return *this == rhs || *this > rhs;
    }

    bool operator <(const Iterator& rhs) const
    {
        return !(*this >= rhs);
    }

    bool operator <=(const Iterator& rhs) const
    {
        return !(*this > rhs);
    }

    T operator*() const
    {
        return *ptr_;
    }

    Iterator& operator++()
    {
        ++ptr_;
        return *this;
    }

    const Iterator operator++(int)
    {
        Iterator temp(*this);
        ++*this;
        return temp;
    }

    Iterator& operator--()
    {
        --ptr_;
        return *this;
    }

    const Iterator operator--(int)
    {
        Iterator temp(*this);
        --*this;
        return temp;
    }

    difference_type operator -(const Iterator &rhs)
    {
        return this->ptr_ - rhs.ptr_;
    }

    T operator [](difference_type n)
    {
        return ptr_[n];
    }

    T operator [](difference_type n) const
    {
        return ptr_[n];
    }

    T &operator *()
    {
        return *ptr_;
    }
};

template <typename T>
Iterator<T> operator +(typename Iterator<T>::difference_type n, const Iterator<T> &it)
{
    Iterator<T> tmp(it);
    tmp += n;
    return tmp;
}

template <typename T>
Iterator<T> operator -(typename Iterator<T>::difference_type n, const Iterator<T> &it)
{
    Iterator<T> tmp(it);
    tmp -= n;
    return tmp;
}
