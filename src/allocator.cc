#include "../include/allocator.h"

template <typename T>
stack_allocator<T>::stack_allocator() {
    capacity = 0;
    ix = 0;
    base_ptr = NULL;
}

template <typename T>
stack_allocator<T>::stack_allocator(size_t cap) {
    capacity = cap;
    ix = 0;

    base_ptr = new T[capacity];
}

template <typename T>
stack_allocator<T>::~stack_allocator() {
    delete[] base_ptr;
}

template <typename T>
bool stack_allocator<T>::reserve(size_t cap) {
    if(capacity) {
        return false;
    }

    capacity = cap;
    base_ptr = new T[capacity];

    return true;
}

template <typename T>
T *stack_allocator<T>::allocate() {
    T *mem = &base_ptr[ix++];

    return mem;
}

template <typename T>
void stack_allocator<T>::pop() {
    if(ix <= 0) {
        return;
    }

    base_ptr[ix - 1] = T();
    ix--;
}

template class stack_allocator<int>;