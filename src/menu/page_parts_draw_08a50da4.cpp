#include "menu/agent36_types.h"

/* A page part with its vtable after 0x24 bytes of data. */
class Part50DA4 {
public:
    char data[0x24];
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();  /* draw */
};

struct Work50DA4 {
    unsigned int flags;       /* +0x0000 */
    int state;                /* +0x0004 */
    char pad_0008[0x6B14];
    Part50DA4 *part_6B1C;     /* +0x6B1C */
    Part50DA4 *part_6B20;     /* +0x6B20 */
    char pad_6B24[0x24];
    Part50DA4 *list;          /* +0x6B48 */
    char pad_6B4C[0xE4C];
    Part50DA4 *grid;          /* +0x7998 */
};

struct Screen50DA4 {
    char pad_00[0x58];
    Work50DA4 *work;          /* +0x58 */
};

extern "C" {
void func_08885220(void *box);
}

extern "C" void func_08A50DA4(Screen50DA4 *self)
{
    if (Flags36::Has(self->work->flags, 2)) {
        return;
    }
    self->work->grid->slot_020();
    self->work->list->slot_020();
    self->work->part_6B1C->slot_020();
    self->work->part_6B20->slot_020();
    func_08885220(D_089D69E8);
    self->work->state = 10;
}
