#include "../stack_allocator.h"

void procedure2() {
    STACK_FRAME

    auto floats = array(int, 32);
    // for (auto& [val, index] : floats) val = 2;
    for (int i = 0; i < floats.count; i++) floats[i] = 2;

    print(__FUNCTION__, floats);
}

array<int> get_array(int size) {
    STACK_FRAME_RETURN(array_fill(int, size, 99));

    return result;
}

void procedure1() {
    STACK_FRAME
    auto floats = array_fill(float, 15, 1.1);

    procedure2();

    printf("%s :", __FUNCTION__);
    // for (auto f : floats) printf("%f ", f);
    for (int i = 0; i < floats.count; i++) printf("%f ", floats[i]);

    printf("\n\n");
    // print(__FUNCTION__, floats);
}

void set_half(array<int>& arr) {
    for (auto& i : arr(0, 10)) i = -1;
}

void procedure() {
    STACK_FRAME

    auto ints = array_fill(int, 50, 7);
    auto arr  = get_array(10);

    set_half(ints);
    // for(int i=0; i<ints(2,4).count; i++)
    //    ints[i] = -10;

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
