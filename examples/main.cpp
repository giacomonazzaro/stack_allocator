#include "../stack_allocator.h"

array<int> make_incremental_array(int size) {
    STACK_FRAME_RETURN(array(int, size));

    for (int i = 0; i < result.count; ++i) result[i] = i;
    return result;
}

void test_procedure() {
    STACK_FRAME

    auto integers = make_incremental_array(1e3);
    int  sum      = 0;
    for (auto i : integers) sum += i;

    printf("sum of first %d integers is: %d\n", integers.count, sum);
}

int main() {
    int bytes = 1e7;
    INIT_STACK_ALLOCATOR(bytes);

    test_procedure();

    DESTROY_STACK_ALLOCATOR();
}
