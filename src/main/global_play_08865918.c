#include "main/agent2_types.h"

extern void func_0899FA64(int, int, int, int, int);

void func_08865918(Agent2GlobalState *self) {
    if (self->field_2CCE == 1) return;
    if (self->field_2CD1) return;
    if (self->field_08C0 >= 0) {
        func_0899FA64(0, self->field_0000[self->field_08C0], 0, 0, 0);
    }
}
