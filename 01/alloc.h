#ifndef TS_ALLOC_H
#define TS_ALLOC_H

#endif //TS_ALLOC_H

#include <cstdlib>

class Allocator
{
private:
    char *alloc_;
    size_t offset_;
    size_t size_;
    bool f;

public:
    Allocator() : alloc_(nullptr), offset_(0), size_(0), f(false) {
    }

    void makeAllocator(size_t maxSize) {
        if (alloc_ != nullptr) {
            if (maxSize > size_) {
                f = true;
            } else {
                offset_ = 0;
                size_ = maxSize;
            }
        } else {
            alloc_ = new char[maxSize];
            size_ = maxSize;
        }
    }

    char* alloc(size_t size) {
        if (f) {
            return nullptr;
        }
        if (offset_ + size > size_) {
            return nullptr;
        }
        offset_ += size;
        return alloc_ + offset_ - size;
    }

    void reset()
    {
        offset_ = 0;
    }

    ~Allocator() {
        delete [] alloc_;
    }
};
