#include "drawing_buffer_packet2.h"
void func_088D53BC(void *object) {
    DrawingPacket2Buffer *self = object;
    if (self->field_0008 && self->field_000C) {
        func_0889F290(D_089D6C30,self->field_0008,self->field_000C);
        func_0889F2D4(D_089D6C30,self->field_0000);
    }
}
void func_088D541C(void *object) {
    DrawingPacket2Buffer *self = object;
    if (self->field_0008 && self->field_000C) func_0889F340(D_089D6C30,0);
}
