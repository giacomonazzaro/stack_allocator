#include "stack_allocator.h"

#include <iostream>
#include <string>

void procedure2() {
    STACK_ALLOCATION
    auto floats = array(int, 32);
    for (auto [val, index] : floats) val = 2;

    print(__FUNCTION__, floats);
}

array<int> get_array(int size) {
    STACK_ALLOCATION_INLINE
    auto result = array_fill(int, size, 99);
    return result;
}

void procedure1() {
    STACK_ALLOCATION
    auto floats = array_fill(float, 15, 1.1);

    procedure2();

    printf("%s :", __FUNCTION__);
    for (auto f : floats) printf("%f ", f);
    printf("\n\n");
    // print(__FUNCTION__, floats);
}

void procedure() {
    STACK_ALLOCATION

    auto ints = array_fill(int, 50, 7);
    auto arr  = get_array(10);

    for (auto& i : ints(2, 4)) {
        i = -10;
    }

    procedure1();

    print(__FUNCTION__, ints);
    print("arr", arr);
}

int main() {
    INIT_STACK_ALLOCATOR(1000);

    // prova();
    procedure();

    DESTROY_STACK_ALLOCATOR();
}
