#include "battle/agent14_types.h"

typedef struct SlotWatch {
    unsigned char active;
    char pad_01[0x3];
    int id;
    char pad_08[0x8];
    unsigned char ready;
    char pad_11[0x3];
    char flags[0x4];
} SlotWatch;

extern int func_08A7E82C(void *flags);
extern SlotTable *func_08AB3274(int id);
extern int func_08A82638(SlotTable *table, unsigned int index);

int func_08AA2054(SlotWatch *self)
{
    if (self->active == 0) {
        return -1;
    }
    func_08A7E82C(self->flags);
    if (func_08A82638(func_08AB3274(self->id), 13) == 0) {
        self->ready = 0;
    }
    return 0;
}
