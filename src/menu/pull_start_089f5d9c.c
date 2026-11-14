#include "menu/agent29_types.h"

/* Screen0B6A0: once the jingle has stopped and the save is ready, starts
   the pull music (D_09B6DA40 +0xC), moves to state 6, clears flag 2 of
   +0x3280 and fades to black. */
typedef struct PullState29 {
    int field_00;
    int field_04;
    int mode;
    int music;       /* +0x0C */
    void *sound;
} PullState29;

extern PullState29 D_09B6DA40;
extern int func_088D5458(MenuSound29 *sound);
extern void func_089FE4CC(Screen0B6A0 *self);
extern void func_088FEB60(int arg);
extern char D_089FE5BC[];

void func_089F5D9C(Screen0B6A0 *self)
{
    MenuSound29 *sound = (MenuSound29 *)self->pad_13C;
    FadeColor29 color;

    if (func_088D5458(sound) != 0 && func_088AC9A8(D_089D712C) != 0) {
        PullState29 *pull;
        func_088D541C(sound);
        func_088D54A4(sound);
        func_089FE4CC(self);
        pull = &D_09B6DA40;
        func_088D5318(sound, pull->music, 0x100000);
        func_088D53BC(sound);
        self->work->state = 6;
        func_088FEB60(1);
        self->work->field_3280 &= ~2;
        func_08873D34((float)func_088D4C5C(), 2, D_089FE5BC, 0, 1, 2);
        color.r = 0.0f;
        color.g = 0.0f;
        color.b = 0.0f;
        color.a = 0.0f;
        func_08873DB0(&color, 2);
    }
}
