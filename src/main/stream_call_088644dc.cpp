#include "main/agent2_stream.h"

extern "C" void func_088644DC(Agent2Stream *self, int id) {
    char name[256];
    self->slot_108(name, id);
    self->slot_0C0(name, 0);
}
