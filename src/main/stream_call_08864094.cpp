#include "main/agent2_stream.h"

extern "C" void func_08864094(Agent2Stream *self, int id, int first,
                              int second, int third, int fourth) {
    char name[256];
    self->slot_108(name, id);
    self->slot_058(name, first, second, third, fourth, 0);
}
