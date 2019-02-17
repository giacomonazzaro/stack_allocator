#include <algorithm>  // @Need std::copy

template <typename Type>
struct array {
    Type* data  = nullptr;
    int   count = 0;

    // array(Type* ptr, int from, int to) {
    // data  = ptr + from;
    // count = to - from;
    // }
    // array(Type* ptr, int size) { array<Type>(ptr, 0, size); }

    inline             operator bool() const { return data == nullptr; }
    inline const Type& operator[](int i) const { return data[i]; }
    inline Type&       operator[](int i) { return data[i]; }
    inline const Type& back() const { return data[count - 1]; }
    inline Type&       back() { return data[count - 1]; }
    inline int         size() { return count; }
    inline void        push_back(const Type& val) { data[count++] = val; }
    inline void        add(const Type& val) { data[count++] = val; }

    inline void insert(const Type& element, int index) {
        assert(index <= count);
        std::copy(&data[index], &data[count], &data[index + 1]);
        data[index] = element;
        count += 1;
    }

    template <int M>
    inline void insert(const array<Type>& arr, int index) {
        assert(index <= count);
        std::copy(&data[index], &data[count], &data[index + arr.count]);
        std::copy(arr, &arr[arr.count], &data[index]);
        count += arr.count;
    }

    inline void fill(const Type& val) {
        for (int i = 0; i < count; ++i) data[i] = val;
    }

    void operator=(const std::initializer_list<Type>& list) {
        count = (int)list.size();
        std::copy(list.begin(), list.end(), data);
    }

    array<Type> operator()(int from, int to) {
        assert(from >= 0 and from <= count);
        assert(to >= 0 and to <= count);
        assert(from <= to);
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
    inline iterator begin() const { return iterator{data, 0}; }
    inline iterator end() const { return iterator{data, count}; }
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

#ifdef SLICE_FROM_VECTOR
template <typename Type>
array make_array(const vector<Type>& vec) {
    return array<const Type>(vec.data(), vec.size());
}

array make_array(vector<Type>& vec) {
    return array<Type>(vec.data(), vec.size());
}
#endif
