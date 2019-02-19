#pragma once

#include "array.h"

struct stack_allocator {
    unsigned char* data     = nullptr;
    int            capacity = 0;
    int            offset   = 0;

    ~stack_allocator() = delete;
};

stack_allocator* default_allocator = nullptr;

inline void init_stack_allocator(stack_allocator*& stack, int size) {
    assert(stack == nullptr);
    stack           = new stack_allocator();
    stack->data     = new unsigned char[size];
    stack->capacity = size;
    stack->offset   = 0;
}

inline void destroy_stack_allocator(stack_allocator* stack) {
    assert(stack != nullptr);
    delete[] stack->data;
    stack->data     = nullptr;
    stack->capacity = 0;
    stack->offset   = 0;
}

unsigned char* allocate(stack_allocator* stack, int bytes) {
    auto ptr = (stack->offset + bytes >= stack->capacity) ?
                   nullptr :
                   stack->data + stack->offset;

    assert(ptr != nullptr);  // Resize stack? Not for now.
    stack->offset += bytes;
    return ptr;
}

template <typename Type>
Type& allocate(stack_allocator* stack) {
    return *((Type*)allocate(stack, sizeof(Type)));
}

template <typename Type>
inline Type& allocate() {
    return allocate<Type>(default_allocator);
}

template <typename Type>
array<Type> allocate_array(int              count,
                           stack_allocator* stack = default_allocator) {
    int  bytes  = sizeof(Type) * count;
    auto result = array<Type>{nullptr, count};
    result.data = (Type*)allocate(stack, bytes);
    return result;
}

template <typename Type>
array<Type> allocate_array(int count, const Type& def,
                           stack_allocator* stack = default_allocator) {
    auto result = allocate_array<Type>(count, stack);
    for (int i = 0; i < count; ++i) result[i] = def;
    return result;
}

struct stack_frame_cleaner {
    stack_allocator* stack = nullptr;
    int              start = 0;

    stack_frame_cleaner(stack_allocator* stack) {
        this->stack = stack;
        start       = stack->offset;
    }

    ~stack_frame_cleaner() { stack->offset = start; }
};

// Used to temporarly allocate local data in stack frames.
#define stack_frame() auto _frame = stack_frame_cleaner(default_allocator);

template <typename Type>
array<Type> copy(const array<Type&> arr,
                 stack_allocator*   stack = default_allocator) {
    auto result = allocate_array<Type>(arr.count, stack);
    memcpy(result.data, arr.data, arr.count);
    return result;
}
