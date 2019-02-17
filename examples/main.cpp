#include "../stack_allocator.h"

array<int> make_incremental_array(int size) {
    // The value of a variable called "result" is allocated on the stack frame
    // of the caller, hence no copy is needed when returning the value.
    STACK_FRAME_RETURN(array(int, size));

    for (int i = 0; i < result.count; ++i) result[i] = i;
    return result;
}

void test_procedure() {
    // Data is allocated by the global stack allocator. Cleanup is automatic at
    // the end of the scope.
    STACK_FRAME;

    auto integers = make_incremental_array(1e3);
    int  sum      = 0;
    for (auto i : integers) sum += i;

    printf("sum of first %d integers is: %d\n", integers.count, sum);
}

int main() {
    // Initialize a global stack allocator
    INIT_STACK_ALLOCATOR(1e7);

    test_procedure();

    DESTROY_STACK_ALLOCATOR();
}