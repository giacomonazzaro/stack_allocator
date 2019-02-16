#pragma once

#include "memory.h"

// namespace giacomo {
struct stack_allocator {
    unsigned char* data     = nullptr;
    int            capacity = 0;
    int            offset   = 0;

    ~stack_allocator() {
        // call destroy_stack_allocator()!
        assert(data == nullptr);
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
    // printf("init stack allocator of size %d\n", (int)size);
}

inline void destroy_stack_allocator(stack_allocator* stack) {
    delete[] stack->data;
    stack->data     = nullptr;
    stack->capacity = 0;
    stack->offset   = 0;
}

struct stack_frame {
    stack_allocator* stack;
    unsigned char*   data   = nullptr;
    int              offset = 0;

    stack_frame(stack_allocator* stack) {
        this->stack = stack;
        data        = stack->data + stack->offset;
        offset      = 0;
    }

    template <typename Type>
    Type& allocate() {
        auto ptr   = (Type*)(data + offset);
        auto bytes = sizeof(Type);
        assert(stack->offset + bytes < stack->capacity);
        if (stack->offset + bytes >= stack->capacity) return *ptr;

        this->offset += bytes;
        stack->offset += bytes;
        return *ptr;
    }

    template <typename Type>
    slice<Type> allocate_array(int count, bool empty) {
        auto bytes  = sizeof(Type) * count;
        auto result = slice<Type>{nullptr, 0};

        assert(stack->offset + bytes < stack->capacity);
        if (stack->offset + bytes >= stack->capacity) return result;
        result.data = (Type*)(data + offset);
        if (not empty) result.count = count;

        this->offset += bytes;
        stack->offset += bytes;
        return result;
    }

    template <typename Type>
    slice<Type> allocate_array(int count, const Type& def) {
        auto result = allocate_array<Type>(count, false);
        if (result.data != nullptr)
            for (int i = 0; i < count; ++i) result[i] = def;
        return result;
    }

    ~stack_frame() {
        stack->offset -= offset;
        data   = nullptr;
        offset = 0;
    }
};

stack_allocator* global_stack_allocator = nullptr;

#define INIT_STACK_ALLOCATOR(N) init_stack_allocator(global_stack_allocator, N);
#define DESTROY_STACK_ALLOCATOR() \
    destroy_stack_allocator(global_stack_allocator);

#define STACK_ALLOCATION auto _frame = stack_frame(global_stack_allocator);

#define array(T, S) _frame.allocate_array<T>(S, false)
#define array_reserve(T, S, Val) _frame.allocate_array<T>(S, true)
#define array_fill(T, S, Val) _frame.allocate_array<T>(S, Val)
#define make(T) _frame.allocate<T>()

// }  // namespace giacomo
