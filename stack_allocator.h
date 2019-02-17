#pragma once

#include "array.h"

struct stack_allocator {
    unsigned char* data     = nullptr;
    int            capacity = 0;
    int            offset   = 0;

    unsigned char* head() { return data + offset; }

    template <typename Type>
    Type& allocate() {
        auto ptr   = (Type*)head();
        auto bytes = sizeof(Type);
        assert(offset + bytes < capacity);
        if (offset + bytes >= capacity) return *ptr;

        offset += bytes;
        return *ptr;
    }

    template <typename Type>
    array<Type> allocate_array(int count, bool empty) {
        auto bytes  = sizeof(Type) * count;
        auto result = array<Type>{nullptr, 0};

        assert(offset + bytes < capacity);
        if (offset + bytes >= capacity) return result;
        result.data = (Type*)head();
        if (not empty) result.count = count;

        offset += bytes;
        return result;
    }

    template <typename Type>
    array<Type> allocate_array_fill(int count, const Type& def) {
        auto result = allocate_array<Type>(count, false);
        if (result.data != nullptr)
            for (int i = 0; i < count; ++i) result[i] = def;
        return result;
    }

    ~stack_allocator() {
        assert(data == nullptr && "call destroy_stack_allocator()!");
    }
};

inline void init_stack_allocator(stack_allocator*& stack, int size) {
    if (stack == nullptr)
        stack = new stack_allocator();
    else
        assert(0);
    stack->data     = new unsigned char[size];
    stack->capacity = size;
    stack->offset   = 0;
}

inline void destroy_stack_allocator(stack_allocator* stack) {
    delete[] stack->data;
    stack->data     = nullptr;
    stack->capacity = 0;
    stack->offset   = 0;
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
#define STACK_ALLOCATION                                \
    auto  _frame = stack_frame(global_stack_allocator); \
    auto& _stack = *global_stack_allocator;

// Used to return data from stack frames
// #define STACK_ALLOCATION_INLINE auto& _stack = *global_stack_allocator;

// allocate single uninitialized struct
#define make(T) _stack.allocate<T>()

// Uninitialized array of capacity S and count S.
#define array(T, S) _stack.allocate_array<T>(S, false)

// Array of capacity S and count S, filled with Val in each element.
#define array_reserve(T, S, Val) _stack.allocate_array<T>(S, true)

// Uninitialized array of capacity S and count 0.
#define array_fill(T, S, Val) _stack.allocate_array_fill<T>(S, Val)

template <typename Type>
array<Type> copy(const array<Type&> arr) {
    STACK_ALLOCATION_INLINE
    auto result = array(Type, arr.count);
    memcpy(result.data, arr.data, arr.count);
    return result;
}
