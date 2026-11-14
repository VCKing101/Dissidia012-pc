#include "battle/auto_region5.h"

/* Runs func_0881BBF4 for ids 0x22..0x3E and func_0881BD58 for ids
 * 0x3F..0x43. */
void func_08B563D0(void)
{
    void *object;
    int id;

    for (id = 0x22; id < 0x29; id++) {
        object = func_0881BAA8();
        func_0881BBF4(object, id);
    }
    for (id = 0x29; id < 0x39; id++) {
        object = func_0881BAA8();
        func_0881BBF4(object, id);
    }
    for (id = 0x39; id < 0x3F; id++) {
        object = func_0881BAA8();
        func_0881BBF4(object, id);
    }
    for (id = 0x3F; id < 0x44; id++) {
        object = func_0881BAA8();
        func_0881BD58(object, id);
    }
}
