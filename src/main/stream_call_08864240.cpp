#include "main/agent2_stream.h"

extern "C" void func_08864240(Agent2Stream *self, int id) {
    char name[256];
    self->slot_108(name, id);
    self->slot_078(name, 0);
}
