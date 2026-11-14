#include "menu/agent29_types.h"

/* Screen0B6A0: while the lamp counter (+0x3224) runs, lights the first
   lamps (handles D_08A0B33C, motion 0x35 then state 0x36) and turns the
   rest off (motion 0x37); when the counter stops, goes on
   (func_089F80F8). Then places the texts. */
extern "C" {
extern const unsigned short D_08A0B33C[];
extern const unsigned short D_08A09B0A[];
int func_089F2758(Screen0B6A0 *self, void *counter);
int func_089F26E0(Screen0B6A0 *self, void *counter);
void func_089F80F8(Screen0B6A0 *self);

void func_089F3B70(Screen0B6A0 *self)
{
    if (func_089F2758(self, (char *)self->work + 0x3224) != 0) {
        int lit = WORK_INT(self->work, 0x3224) + func_089F26E0(self, (char *)self->work + 0x3224);
        for (int i = 0; i < 5; i++) {
            MenuModel *lamp = self->work->handles[D_08A0B33C[i]];
            short state = lamp->field_0C;
            if (i < lit) {
                if (state == -1 || state == 0x37) {
                    func_088D9F5C(lamp, 0x35, 1);
                    MENU_MODEL_STATE(self->work->handles[D_08A0B33C[i]]) = 0x36;
                }
            } else if (state != -1 && state != 0x37) {
                func_088D9F5C(lamp, 0x37, 1);
            }
        }
        if (func_089F2758(self, (char *)self->work + 0x3224) == 0) {
            func_089F80F8(self);
        }
    }
    func_088D44EC(self->work->handles, self->work->texts, D_08A09B0A);
}
}
