#include "drawing_buffer_packet2.h"
int func_0889F5E8(void *object, int key) {
    DrawingPacket2Context *self = object;
    int count = self->field_0004->count_0008;
    int index = 0;
    if (index < count) {
        DrawingPacket2Info *record = self->field_0008;
        do {
            if (record->key_0000 == key) return index;
            index++;
            record++;
        } while (index < count);
    }
    return -1;
}
