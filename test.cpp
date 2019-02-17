#include <stdio.h>
#include "array_algorithms.h"
#include "stack_allocator.h"

// #include "he_string.h"
// #include <vector>

void test_stack_array() {
    STACK_ALLOCATION
    // array<int, 10> a = {0, 1, 2, 3};
    auto a = array(int, 100);
    a      = {0, 1, 2, 3};

    print("initializer_list {0, 1, 2, 3}", a);
    a.add(40);
    a.add(50);
    a.add(70);
    print("adding 40, 50, 70", a);

    a.insert(60, 6);
    print("inserting 60 into 6", a);

    // a.pop();
    // print("pop", a);

    // print("indices_of 3", a.indices_of(3));

    print("slice 3:7", a(3, 7));

    a(0, 5).fill(-1);
    print("slice(0, 5) = -1", a);
    printf("end test_stack_array()\n");
}

void test_heap_array() {
    STACK_ALLOCATION
    // Array<int> a = {7, 9, 3, 8, 0, 2, 4, 8, 3, 9};
    auto a = array(int, 10);
    a      = {7, 9, 3, 8, 0, 2, 4, 8, 3, 9};

    print("Init", a);

    // a.insert(7, 5);
    // print("Insert 7 at 5", a);

    auto b = array(int, 3);
    b      = {-1, -1, -1};
    print("b = {-1, -1, -1}", b);

    /*a(4, 7) = b;
    print("slice 4:7 = b", a);*/

    quick_sort(a(0, 6));
    print("sort 0:6", a);
}

void test_interaction() {
    STACK_ALLOCATION
    auto heap = array(int, 10);
    heap      = {0, -1, -2, -3, -4, -5, -6, -7, -8, -9};

    print("heap", heap);

    auto stack = heap(1, heap.count - 1 - 1);
    print("stack", stack);
    reverse(stack);
    print("reverse(stack)", stack);

    /*heap(1, heap.count - 1 - 1) = stack;
    print("heap(1, -1) = stack", heap);*/

    auto sstack = heap(0, 3);
    print("sstack", sstack);

    printf("end test_interaction()\n");
}

void test_sorting() {
    STACK_ALLOCATION
    // Timer time;
    auto quick = array(int, 10);
    // printf("allocating %d int in %lf seconds\n", count(x), time.secs());
    for (int i = 0; i < quick.count; ++i) {
        quick[i] = rand() % quick.count;
    }

    /*auto bubble = copy(quick);
    auto heap   = copy(quick);

    // time.reset();
    // printf("sorting...\n");
    quick_sort(quick);
    bubble_sort(bubble);
    heap_sort(heap);

    print("quick_sort", quick);
    print("bubble_sort", bubble);
    print("heap_sort", heap);

    // printf("done in %lf seconds\n", time.secs());*/
}

int main() {
    INIT_STACK_ALLOCATOR(10000);

    test_sorting();
    test_heap_array();
    test_stack_array();
    test_interaction();
    // test_conversion();

    DESTROY_STACK_ALLOCATOR();
}
