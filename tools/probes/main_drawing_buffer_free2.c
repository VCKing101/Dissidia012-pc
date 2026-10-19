#include "drawing_buffer_packet2.h"
void func_088D5528(void *object, void *allocator) {
    DrawingPacket2Buffer *self = object;
    if (self->field_0008 && self->field_000C) {
        func_0889F340(D_089D6C30,0);
        func_0889F2B8(D_089D6C30);
        if (self->field_0008) {
            func_0885C538(allocator,self->field_0008,0x80);
            self->field_0008 = 0;
        }
    }
}
