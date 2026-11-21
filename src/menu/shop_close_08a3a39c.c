#include "menu/agent36_types.h"

/* Item list screen: closes the screen once its loads are done: frees the
 * slots, texts and models, plays the closing layout and hands the chosen
 * item on (to func_08A4D76C or the profile). */

extern ScriptCmd36 D_08A3B758[];
int func_08A39E1C(ShopScreen36 *self);
void func_08A39EE4(ShopScreen36 *self);
void func_08A39330(ShopScreen36 *self);
void func_088D3A5C(void *layout, ScriptCmd36 *script, int count);
void func_08A4D76C(int item);

void func_08A3A39C(ShopScreen36 *self)
{
    int i;

    if (func_08A39E1C(self) != 0) {
        return;
    }
    func_08A39EE4(self);
    func_08A39330(self);
    for (i = 0; i < 0x21; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
    for (i = 0; i < 0x6C; i++) {
        if (self->work->handles[i] != 0) {
            func_088DA060(self->work->handles[i]);
            self->work->handles[i] = 0;
        }
    }
    func_08A418E8(D_08A457EC);
    func_088D3A5C(self->work->layout, D_08A3B758, 0x13);
    if (self->mode == 1) {
        func_08A4D76C(self->work->item);
    } else {
        Profile36 *profile = &D_089D712C->profile;
        profile->item = self->work->item;
    }
    self->work->field_1250 = 0;
    self->work->state = 3;
}
