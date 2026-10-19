#include "drawing_buffer_packet2.h"
struct ResourcePacket2Records {
    static int count(DrawingPacket2Context *context) { return context->field_0004->count_0008; }
    static DrawingPacket2Info *at_byte_offset(DrawingPacket2Context *context, int offset) {
        return reinterpret_cast<DrawingPacket2Info *>(reinterpret_cast<unsigned char *>(context->field_0008)+offset);
    }
};
extern "C" int func_0889F5E8(DrawingPacket2Context *self, int key) {
    int count = ResourcePacket2Records::count(self);
    int index = 0;
    if (index < count) {
        DrawingPacket2Info *record = ResourcePacket2Records::at_byte_offset(self,0);
        do {
            if (record->key_0000 == key) goto found;
            index++;
            record++;
        } while (index < count);
    }
    index = -1;
found:
    return index;
}
