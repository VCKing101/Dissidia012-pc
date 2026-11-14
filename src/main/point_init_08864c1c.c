#include "main/agent2_types.h"

extern float D_089C9328;
extern float D_089C932C;

Agent2Point *func_08864C1C(Agent2Point *self) {
    self->field_0000 = D_089C9328;
    self->field_0004 = D_089C932C;
    self->field_0008 = 0xFF808080;
    return self;
}
