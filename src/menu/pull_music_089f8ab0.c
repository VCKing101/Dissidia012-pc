#include "menu/agent29_types.h"

/* Screen0B6A0: once func_088FEA4C reports the jingle done, plays the
   pull music (D_09B6DA40 +0xC) and returns to the saved state (+0x327C). */
typedef struct PullState29 {
    int field_00;
    int field_04;
    int mode;
    int music;       /* +0x0C */
    void *sound;
} PullState29;

extern PullState29 D_09B6DA40;
extern int func_088FEA4C(void);

void func_089F8AB0(Screen0B6A0 *self)
{
    if (func_088FEA4C() != 0) {
        return;
    }
    {
        PullState29 *pull = &D_09B6DA40;
        MenuSound29 *sound = (MenuSound29 *)self->pad_13C;
        func_088D5318(sound, pull->music, 0x100000);
        func_088D53BC(sound);
    }
    self->work->state = WORK_INT(self->work, 0x327C);
}
