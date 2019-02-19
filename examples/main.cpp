#include "../stack_allocator.h"

array<int> make_incremental_array(int size) {
    // If an array is initialized without calling stack_frame(), the data is
    // allocated on the stack frame of the caller, hence no copy is needed when
    // returning the value.
    auto result = allocate_array<int>(size);

    for (int i = 0; i < result.count; ++i) result[i] = i;
    return result;
}

void test_procedure() {
    // Data is allocated by default_allocator. Cleanup is automatic at
    // the end of the scope.
    stack_frame();

    int  size     = 1e3;
    auto ones     = allocate_array_fill(size, 1);
    auto integers = make_incremental_array(size);
    int  sum      = 0;
    for (int i = 0; i < ones.count; ++i) {
        integers[i] += ones[i];
    }

    for (auto i : integers) sum += i;
    sum -= size;

    printf("sum of first %d integers is: %d\n", size, sum);
    assert(sum == size * (size - 1) / 2);
}

int main() {
    // Initialize a global stack allocator
    init_stack_allocator(default_allocator, 1e7);

    test_procedure();

    destroy_stack_allocator(default_allocator);
}