#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "commons.h"

template <typename T>
class stack_allocator {
private:
    T *base_ptr;

    size_t ix;
    size_t capacity;

public:
    stack_allocator();
    stack_allocator(size_t cap);

    ~stack_allocator();

    bool reserve(size_t cap);

    T *allocate();

    void pop();
};

#endif //__ALLOCATOR_H__