#include "main/agent2_types.h"
extern "C" void func_0886615C(Agent2GlobalState *);
extern "C" void func_0888218C(int, int);
extern "C" void func_08866A3C(Agent2GlobalState *);
extern "C" void func_08866968(Agent2GlobalState *);
extern "C" void func_0886676C(Agent2GlobalState *self) {
    func_0886615C(self);
    for (int index = 0; index < 2; index++) {
        func_0888218C(self->field_0000[index], 0x80);
        self->field_0000[index] = 0;
    }
    func_08866A3C(self);
    func_08866968(self);
}
