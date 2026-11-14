#include "menu/agent29_types.h"

/* ScreenE75C4: takes the slot count from D_089D76B0, clears the cursor
   and allocates one word per slot from the screen's heap. */
extern "C" {
typedef struct Global9D76B0 {
    char pad_00[0x8];
    int count;
} Global9D76B0;

extern Global9D76B0 *D_089D76B0;
}

extern "C" void func_089E3864(ScreenE75C4 *self)
{
    WorkE75C4 *work = self->work;
    work->field_F70 = D_089D76B0->count;
    self->work->field_F74 = 0;
    self->work->field_F78 = 0;
    self->work->field_F7C = Heap29::Alloc(self->heap, self->work->field_F70 << 2, true);
}
