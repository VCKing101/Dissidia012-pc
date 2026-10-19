#include "drawing_buffer_packet2.h"
int func_0889F2D4(void *object, int key) {
    DrawingPacket2Info *info = func_0889F3E4(object,key);
    if (info) {
        float value = (float)info->field_000D * 0.01f;
        func_0889F750((unsigned char *)object+0x1C,0,0,0,value);
        return 1;
    }
    return 0;
}
