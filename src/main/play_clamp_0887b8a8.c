#include "main/agent10_types.h"

void func_0887B8A8(Agent10Player *self, int value) {
    if (value > 0xFF) {
        value = 0xFF;
    } else if (value < -1) {
        value = -1;
    }
    self->field_0A = value;
}
