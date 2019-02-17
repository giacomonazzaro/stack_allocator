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

unsigned char* allocate(stack_allocator& stack, int bytes) {
    auto ptr = (stack.offset + bytes >= stack.capacity) ?
                   nullptr :
                   stack.data + stack.offset;

    assert(ptr != nullptr);  // Resize stack? Not for now.
    stack.offset += bytes;
    return ptr;
}

template <typename Type>
Type& allocate_struct(stack_allocator& stack) {
    return *((Type*)allocate(stack, sizeof(Type)));
}

template <typename Type>
array<Type> allocate_array(stack_allocator& stack, int count, bool empty) {
    int  bytes  = sizeof(Type) * count;
    auto result = array<Type>{nullptr, 0};
    result.data = (Type*)allocate(stack, bytes);
    if (result.data and not empty) result.count = count;
    return result;
}

template <typename Type>
array<Type> allocate_array_fill(stack_allocator& stack, int count,
                                const Type& def) {
    auto result = allocate_array<Type>(stack, count, false);
    for (int i = 0; i < count; ++i) result[i] = def;
    return result;
}

struct stack_frame {
    stack_allocator* stack = nullptr;
    int              start = 0;

    stack_frame(stack_allocator* stack) {
        this->stack = stack;
        start       = stack->offset;
    }

    ~stack_frame() { stack->offset = start; }
};

// Global variable, used for macros.
stack_allocator* global_stack_allocator = nullptr;

// Init and destroy global_stack_allocator.
#define INIT_STACK_ALLOCATOR(N) init_stack_allocator(global_stack_allocator, N);
#define DESTROY_STACK_ALLOCATOR() \
    destroy_stack_allocator(global_stack_allocator);

// Used to temporarly allocate local data in stack frames.
#define STACK_FRAME                                     \
    auto  _frame = stack_frame(global_stack_allocator); \
    auto& _stack = *global_stack_allocator;

#define STACK_FRAME_RETURN(init)            \
    auto& _stack = *global_stack_allocator; \
    auto  result = init;                    \
    auto  _frame = stack_frame(global_stack_allocator);

// allocate single uninitialized struct
#define make(T) allocate<T>(_stack)

// Uninitialized array of capacity S and count S.
#define array(T, S) allocate_array<T>(_stack, S, false)

// Array of capacity S and count S, filled with Val in each element.
#define array_empty(T, S) allocate_array<T>(_stack, S, true)

// Uninitialized array of capacity S and count 0.
#define array_fill(T, S, Val) allocate_array_fill<T>(_stack, S, Val)

template <typename Type>
array<Type> copy(const array<Type&> arr) {
    STACK_FRAME_RETURN(array(Type, arr.count))
    memcpy(result.data, arr.data, arr.count);
    return result;
}
