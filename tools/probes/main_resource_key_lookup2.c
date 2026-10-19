#include "drawing_buffer_packet2.h"
DrawingPacket2Info *func_0889F3E4(void *object, int key) {
    DrawingPacket2Context *self = object;
    int index = func_0889F5E8(self,key);
    if (index >= 0) return self->field_0008 + index;
    return 0;
}
