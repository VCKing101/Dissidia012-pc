#include "main/agent2_stream.h"

extern "C" void func_088645DC(Agent2Stream *self, int id, void *callback,
                              void *argument) {
    char name[256];
    self->slot_108(name, id);
    self->slot_0D8(name, callback, argument, 0);
}
