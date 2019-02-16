template <typename Type>
struct slice {
    Type* data  = nullptr;
    int   count = 0;

    // operator Type*() { return data; }
    const Type& operator[](int i) const { return data[i]; }
    Type&       operator[](int i) { return data[i]; }
    const Type& back() const { return data[count - 1]; }
    Type&       back() { return data[count - 1]; }
    void        push_back(const Type& val) { data[count++] = val; }

    slice operator()(int from, int to) {
        assert(from >= 0 and from <= count);
        assert(to >= 0 and from + to <= count);
        return {data + from, to - from};
    }
    
    const slice operator()(int from, int to) const {
        assert(from >= 0 and from <= count);
        assert(to >= 0 and from + to <= count);
        return {data + from, to - from};
    }

    struct iterator {
        Type*     data;
        int       i;
        iterator& operator++() {
            i += 1;
            return *this;
        }
        bool        operator!=(const iterator& other) { return i != other.i; }
        Type&       operator*() { return *(data + i); }
        const Type& operator*() const { return *(data + i); }
    };
    iterator begin() const { return iterator{data, 0}; }
    iterator end() const { return iterator{data, count}; }
};

template <typename Container>
inline void print(const char* name, Container&& a, int line_size = 32,
                  int max_elems = 300) {
    int count = a.count;
    printf("%s (count: %d)\n", name, count);  // capacity(a));
    if (count)
        printf(" ");
    else {
        printf("\n");
        return;
    }
    for (int i = 0; i < count; ++i) {
        printf("%d", a[i]);
        if (i == count - 1)
            printf("\n\n");

        else if (i % line_size == line_size - 1)
            printf("\n ");
        else
            printf(", ");

        if (i > max_elems) {
            printf("...\n\n");
            return;
        }
    }
}
