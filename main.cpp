#include <iostream>
#include <string>
#include "allocator.h"

void procedure2() {
    STACK_ALLOCATION
    auto ints = array(int, 32);
    for (int i = 0; i < ints.count; ++i) ints[i] = 2;

    print(__FUNCTION__, ints);
}

void procedure1() {
    STACK_ALLOCATION
    auto floats = array_fill(float, 15, 1.1);

    procedure2();

    for (auto f : floats) printf("%f ", f);
    printf("\n");
    // print(__FUNCTION__, floats);
}

void printa(const slice<int>& data) {
    auto head = data(0, 1);
    head[0]   = 144;
    for (auto i : data) printf("%d \n", i);
    printf("\n");
}

void prova() {
    STACK_ALLOCATION

    auto data = array_fill(int, 10, 99);
    printa(data);
}

void procedure() {
    STACK_ALLOCATION

    auto ints = array(int, 50);
    printf("count: %d\n", ints.count);
    for (auto& i : ints) i = 7;

    for (auto& i : ints(2, 4)) i = -10;

    procedure1();

    print(__FUNCTION__, ints);
}

int main() {
    INIT_STACK_ALLOCATOR(1000);

    prova();
    // procedure();

    DESTROY_STACK_ALLOCATOR();
}
