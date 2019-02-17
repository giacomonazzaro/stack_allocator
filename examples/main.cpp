#include "../stack_allocator.h"

array<int> make_incremental_array(int size) {
    // The variable "result" is allocated on the stack frame of the caller (to
    // achieve "return value optimization")
    STACK_FRAME_RETURN(array(int, size));

    for (int i = 0; i < result.count; ++i) result[i] = i;
    return result;
}

void test_procedure() {
    // Data is allocated on global stack. Cleanup is automatic
    STACK_FRAME

    auto integers = make_incremental_array(1e3);
    int  sum      = 0;
    for (auto i : integers) sum += i;

    printf("sum of first %d integers is: %d\n", integers.count, sum);
}

int main() {
    // Initialize global stack allocator
    INIT_STACK_ALLOCATOR(1e7);

    test_procedure();

    DESTROY_STACK_ALLOCATOR();
}