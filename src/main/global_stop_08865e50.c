#include "main/agent2_types.h"

extern Agent2GlobalState *D_089D5EC0;
extern void func_089A00D8(void);
extern void func_08865918(Agent2GlobalState *);

void func_08865E50(Agent2GlobalState *self) {
    if (!self->field_2CCE) return;
    self->field_2CCE = 0;
    if (self->field_08C0 >= 0) func_089A00D8();
}

void func_08865E98(void) {
    if (D_089D5EC0->field_2CD0) return;
    func_08865918(D_089D5EC0);
}
