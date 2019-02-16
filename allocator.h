#pragma once

template <typename Type>
struct slice {
    Type* data;
    int   count;

    // operator Type*() { return data; }
    const Type& operator[](int i) const { return data[i]; }
    Type&       operator[](int i) { return data[i]; }
};

struct allocator {
    allocator*     arena  = nullptr;
    unsigned char* data   = nullptr;
    int            size   = 0;
    int            offset = 0;

    allocator stack_frame() {
        // printf("creating stack frame of size %d\n", (int)0);
        allocator frame;
        frame.arena  = this;
        frame.data   = data + offset;
        frame.size   = 0;
        frame.offset = 0;
        return frame;
    }

    template <typename Type>
    slice<Type> allocate(int s, bool set_to_zero = true) {
        auto result = slice<Type>{(Type*)(data + offset), s};

        auto bytes = sizeof(Type) * s;
        if (set_to_zero) memset(result.data, 0, bytes);

        offset += bytes;
        if (arena != nullptr) arena->offset += bytes;

        return result;
    }

    void free() { offset = 0; }

    ~allocator() {
        if (arena == nullptr) {
            // printf("destroying arena of size %d\n", (int)size);
            if (data != nullptr) delete[] data;
        } else {
            // printf("destroying stack frame of size %d\n", (int)size);
            arena->offset -= offset;
        }
        data   = nullptr;
        size   = 0;
        offset = 0;
    }
};

inline void init_allocator(allocator& alloc, int s) {
    alloc.arena  = nullptr;
    alloc.data   = new unsigned char[s];
    alloc.size   = s;
    alloc.offset = 0;
    // printf("creating arena of size %d\n", (int)size);
}

inline void destroy_allocator(allocator& alloc) { alloc.~allocator(); }
