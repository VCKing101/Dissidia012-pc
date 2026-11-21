#include "menu/agent36_types.h"

/* Item list screen: steps the loading slots: shows a loaded one, waits on
 * a loading one, or starts the first idle one. */

extern "C" {
void func_08A392E8(ShopScreen36 *self);
void func_08A3A830(void);
void func_088629DC(void *request, int a, int b, void (*done)(void), ShopSlot36 *slot, int one);
}

extern "C" void func_08A39D2C(ShopScreen36 *self)
{
    ShopWork36 *work;
    ShopSlot36 *slot;
    int state;
    int i;

    work = self->work;
    for (i = 0; i < work->slot_count; i++) {
        slot = &work->slots[i];
        state = slot->state;
        if (state == 2) {
            func_08A392E8(self);
            slot->state = 3;
            state = 3;
        }
        if (state == 1) {
            break;
        }
        if (state == 0) {
            func_088629DC(slot->request, slot->field_04, slot->field_08, func_08A3A830, slot, 1);
            slot->state = 1;
            break;
        }
        work = self->work;
    }
}
