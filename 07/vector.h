#ifndef HW7_VECTOR_H
#define HW7_VECTOR_H

#endif //HW7_VECTOR_H

#include <cstdlib>
#include <stdexcept>

#include "allocator.h"
#include "iterator.h"

template <typename T, class Alloc = Allocator<T>>
class Vector
{
private:
    size_t length_;
    size_t capacity_;
    T *mem_;

    void alloc_check()
    {
        if (length_ == capacity_) {
            T *old = mem_;
            if (capacity_ == 0) {
                capacity_ = 10;
            } else {
                capacity_ *= 2;
            }
            mem_ = Alloc().allocate(capacity_);
            if (mem_ == nullptr) {
                throw std::runtime_error("Cannot allocate memory");
            }
            for (size_t i = 0; i < length_; i++) {
                mem_[i] = old[i];
            }
            Alloc().deallocate(old);
        }
    }

public:
    typedef T value_type;
    typedef Iterator<T> iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;


    Vector() noexcept: length_(0), capacity_(0), mem_(nullptr)
    {
    }

    Vector(size_t size, const T &value, const Alloc &alloc = Alloc()): length_(size), capacity_(size)
    {
        mem_ = alloc.allocate(capacity_);
        for (size_t i = 0; i < length_; i++) {
            mem_[i] = value;
        }
    }

    Vector(size_t size, const Alloc &alloc = Alloc()): length_(size), capacity_(size)
    {
        mem_ = alloc.allocate(capacity_);
    }

    Vector(iterator first, iterator last, const Alloc &alloc = Alloc()) {
        length_ = 0;
        for (iterator i = first; i != last; i++) {
            length_++;
        }
        capacity_ = length_;
        mem_ = alloc.allocate(capacity_);
        for (auto i = first; i < last; ++i) {
            mem_[i - first] = *i;
        }
    }

    Vector(const Vector &other): length_(other.length_), capacity_(other.capacity_) {
        mem_ = Alloc().allocate(capacity_);
        for (size_t i = 0; i < length_; i++) {
            mem_[i] = other[i];
        }
    }

    Vector(Vector &&other) noexcept: length_(other.length_), capacity_(other.capacity_)
    {
        mem_ = std::move(other.mem_);
        other.length_ = 0;
        other.capacity_ = 0;
        other.mem_ = nullptr;
    }

    Vector(std::initializer_list<T> list, const Alloc &alloc = Alloc()): length_(list.size()), capacity_(list.size())
    {
        mem_ = alloc.allocate(capacity_);
        for (auto i = list.begin(); i < list.end(); ++i) {
            mem_[i - list.begin()] = *i;
        }
    }

    Vector &operator =(const Vector &rhs) {
        if (mem_ != rhs.mem_) {
            Alloc().deallocate(mem_);
            length_ = rhs.length_;
            capacity_ = rhs.capacity_;
            mem_ = Alloc().allocate(capacity_);
            for (size_t i = 0; i < length_; i++) {
                mem_[i] = rhs[i];
            }
        }
        return *this;
    }

    Vector &operator =(Vector &&rhs) noexcept
    {
        Alloc().deallocate(mem_);
        length_ = rhs.length_;
        capacity_ = rhs.capacity_;
        mem_ = std::move(rhs.mem_);
        rhs.size = 0;
        rhs.capacity_ = 0;
        rhs.mem_ = nullptr;
        return *this;
    }

    Vector &operator =(std::initializer_list<T> list) {
        length_ = list.size();
        capacity_ = list.size();
        for (auto i = list.begin(); i < list.end(); ++i) {
            mem_[i - list.begin()] = *i;
        }
        return *this;
    }

    T &operator[](size_t index)
    {
        if (index >= length_) {
            throw std::runtime_error("Index out of range");
        }
        return mem_[index];
    }

    const T &operator[](size_t index) const
    {
        if (index >= length_) {
            throw std::runtime_error("Index out of range");
        }
        return mem_[index];
    }

    void push_back(const T &elem)
    {
        alloc_check();
        length_++;
        mem_[length_ - 1] = elem;
    }

    void push_back(T &&elem)
    {
        alloc_check();
        length_++;
        mem_[length_ - 1] = std::move(elem);
    }

    void pop_back()
    {
        if (length_ == 0) {
            throw std::runtime_error("No elements to pop");
        }
        length_--;
    }

    template <typename... ArgsT>
    void emplace_back(ArgsT&... args)
    {
        alloc_check();
        length_++;
        T *elem = new (mem_ + length_) T(std::forward<ArgsT>(args)...);
        mem_[length_ - 1] = *elem;
    }

    bool empty() const
    {
        return length_ == 0;
    }

    size_t size() const
    {
        return length_;
    }

    void clear()
    {
        length_ = 0;
    }

    iterator begin()
    {
        return iterator(mem_);
    }

    iterator end()
    {
        return iterator(mem_ + length_);
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(iterator(mem_ + length_));
    }

    reverse_iterator rend()
    {
        return reverse_iterator(iterator(mem_));
    }

    void resize(size_t count, const T &value)
    {
        if (length_ >= count) {
            length_ = count;
        } else {
            if (capacity_ < count) {
                T *old = mem_;
                capacity_ = 2 * count;
                mem_ = Alloc().allocate(capacity_);
                if (mem_ == nullptr) {
                    throw std::runtime_error("Cannot allocate memory");
                }
                for (size_t i = 0; i < length_; i++) {
                    mem_[i] = std::move(old[i]);
                }
                Alloc().deallocate(old);
            }
            for (size_t i = length_; i < count; i++) {
                mem_[i] = value;
            }
            length_ = count;
        }
    }

    void reserve(size_t new_cap)
    {
        if (capacity_ < new_cap) {
            T *old = mem_;
            capacity_ = new_cap;
            mem_ = Alloc().allocate(capacity_);
            for (size_t i = 0; i < length_; i++) {
                mem_[i] = std::move(old[i]);
            }
        }
    }

    size_t capacity() const
    {
        return capacity_;
    }

    ~Vector()
    {
        Alloc().deallocate(mem_);
    }
};
