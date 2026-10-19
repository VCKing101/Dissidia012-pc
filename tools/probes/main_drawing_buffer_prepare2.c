#include "drawing_buffer_packet2.h"
void func_088D5318(void *object, int key, int capacity) {
    DrawingPacket2Buffer *self = object;
    char path[256];
    self->field_0000 = key;
    self->field_0004 = func_0889F3E4(D_089D6C30,key);
    DrawingPacket2Info *info = self->field_0004;
    if (info) {
        func_0893476C(path,D_089CC0A4,info->path_001C);
        self->field_000C = func_08862C40(path,1);
        int bytes = self->field_000C;
        if (capacity >= bytes && self->field_0008) {
            func_08862854(path,self->field_0008,bytes,1);
        } else {
            self->field_0008 = 0;
            self->field_000C = 0;
        }
    }
}
