#include "main/agent2_stream.h"

extern "C" void func_08864374(Agent2Stream *self, int id, int first,
                              int second) {
    char name[256];
    self->slot_108(name, id);
    self->slot_0A0(name, first, second, 0);
}
