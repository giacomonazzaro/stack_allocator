#pragma once

#include "array.h"

struct stack_allocator {
    unsigned char* data     = nullptr;
    int            capacity = 0;
    int            offset   = 0;

    ~stack_allocator() {
        assert(data == nullptr && "call destroy_stack_allocator()!");
    }
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
array<Type> allocate_array(stack_allocator* stack, int count, bool empty) {
    int  bytes  = sizeof(Type) * count;
    auto result = array<Type>{nullptr, 0};
    result.data = (Type*)allocate(stack, bytes);
    if (result.data and not empty) result.count = count;
    return result;
}

template <typename Type>
inline array<Type> allocate_array(int count, bool empty = false) {
    return allocate_array<Type>(default_allocator, count, empty);
}

template <typename Type>
array<Type> allocate_array_fill(
    stack_allocator* stack, int count, const Type& def) {
    auto result = allocate_array<Type>(stack, count, false);
    for (int i = 0; i < count; ++i) result[i] = def;
    return result;
}

template <typename Type>
array<Type> allocate_array_fill(int count, const Type& def) {
    auto result = allocate_array<Type>(count, false);
    for (int i = 0; i < count; ++i) result[i] = def;
    return result;
}

struct _stack_frame {
    stack_allocator* stack = nullptr;
    int              start = 0;

    _stack_frame(stack_allocator* stack) {
        this->stack = stack;
        start       = stack->offset;
    }

    ~_stack_frame() { stack->offset = start; }
};

// Used to temporarly allocate local data in stack frames.
#define stack_frame() auto _frame = _stack_frame(default_allocator);

template <typename Type>
array<Type> copy(const array<Type&> arr) {
    auto result = allocate_array<Type>(arr.count);
    memcpy(result.data, arr.data, arr.count);
    return result;
}
