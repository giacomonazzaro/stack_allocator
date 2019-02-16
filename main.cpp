#include <iostream>
#include <string>
#include "allocator.h"

allocator arena;

#define STACK_ALLOCATION auto _frame = arena.stack_frame();

#define array(T, S) _frame.allocate<T>(S)

void procedure2() {
    STACK_ALLOCATION
    auto ints = array(int, 4);
    for (int i = 0; i < ints.count; ++i) ints[i] = 2;

    for (int i = 0; i < ints.count; ++i) {
        printf("-- -- %d: %d\n", i, ints[i]);
    }
}

void procedure1() {
    STACK_ALLOCATION
    // auto frame = arena.stack_frame();
    // auto ints = frame.allocate<int>(4, false);
    auto ints = array(int, 4);

    procedure2();
    for (int i = 0; i < ints.count; ++i) ints[i] = 1;

    for (int i = 0; i < ints.count; ++i) {
        printf("--%d: %d\n", i, ints[i]);
    }
}

void procedure() {
    STACK_ALLOCATION

    auto ints = array(int, 5);
    printf("count: %d\n", ints.count);
    for (int i = 0; i < ints.count; ++i) ints[i] = 7;

    procedure1();

    for (int i = 0; i < ints.count; ++i) {
        printf("%d: %d\n", i, ints[i]);
    }
}

int main() {
    init_allocator(arena, 10000);
    procedure();
    destroy_allocator(arena);
}
