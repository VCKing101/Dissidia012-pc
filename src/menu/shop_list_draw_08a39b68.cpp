#include "menu/agent36_types.h"

/* Item list screen: redraws the list (clearing the shown marks first when
 * reset is set): cursor, rows, texts and scroll arrows. */

extern "C" {
extern ScriptCmd36 D_08A3BD10[];
void func_08A3AC24(ShopScreen36 *self);
void func_08A392E8(ShopScreen36 *self);
void func_08A39980(ShopScreen36 *self);
void func_08A3938C(ShopScreen36 *self, int row);
}

extern "C" void func_08A39B68(ShopScreen36 *self, unsigned char reset)
{
    int i;

    if (reset) {
        for (i = 0; i < 0x21; i++) {
            self->work->shown[i] = 0xFFFF;
        }
        func_08935828(self->work->marks, 0xFF, 0x40);
    }
    func_08A3AC24(self);
    ShopWork36 *work = self->work;
    func_088D4C64(&work->cursor, work->texts, work->shown, 0, 7, 8);
    func_08A392E8(self);
    func_08A39980(self);
    for (i = 0; i < 4; i++) {
        func_08A3938C(self, i);
    }
    func_088D428C(self->work->handles, work->texts, D_08A3BD10, 1);
    func_088D4A68(self->work->arrows, &self->work->cursor, self->work->handles[0x69],
                  self->work->handles[0x6B], 1, 0x14, 0);
}
