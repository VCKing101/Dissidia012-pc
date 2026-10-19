#include "drawing_buffer_packet2.h"
int func_088D5458(void *object) {
    DrawingPacket2Buffer *self = object;
    if (self->field_0008 && self->field_000C) {
        if (!func_0889F35C(D_089D6C30)) return 1;
    }
    return 0;
}
void func_088D54A4(void *object) {
    DrawingPacket2Buffer *self = object;
    if (self->field_0008 && self->field_000C) func_0889F2B8(D_089D6C30);
}
