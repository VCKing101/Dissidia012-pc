#include "main/agent2_types.h"

void func_08866010(Agent2GlobalState *self, unsigned char red,
                   unsigned char green, unsigned char blue) {
    unsigned char alpha = 0xFF;
    self->field_08BC = (((((alpha << 8) + blue) << 8) + green) << 8) + red;
}
