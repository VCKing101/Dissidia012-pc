#include "main/agent2_stream.h"

extern "C" int func_088626AC(char *);

extern "C" void func_0886402C(Agent2Stream *self, int unused, int first,
                              int second) {
    char name[256];
    int value = func_088626AC(name);
    self->slot_030(name, first, second, value);
}
