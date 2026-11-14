#include "main/agent2_stream.h"

extern "C" int func_088626AC(char *);

extern "C" void func_088641B8(Agent2Stream *self, int unused, int first,
                              int second, int third, int fourth) {
    char name[256];
    int value = func_088626AC(name);
    self->slot_058(name, first, second, third, fourth, value);
}
