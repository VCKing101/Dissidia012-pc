#include "main/agent2_stream.h"

extern "C" int func_088626AC(char *);

extern "C" void func_0886432C(Agent2Stream *self) {
    char name[256];
    func_088626AC(name);
    self->slot_078(name, 1);
}
