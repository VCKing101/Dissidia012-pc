#include "menu/agent29_types.h"

/* Screen1AB08: the two pick lists of the options screen. Both read the
   option block at +0x652C of the root's work. */
typedef struct Options29 {
    unsigned short flags;
    char pad_02[0x16];
    short field_18; /* the chosen item of the first list */
    char pad_1A[0xC0];
    short field_DA; /* the chosen item of the second list */
} Options29;

extern int func_088CC22C(void);
extern int func_088CC3B0(int item);
extern int func_088CC364(int item);
extern int func_08A20954(MenuRoot *root);
extern int func_08A20960(MenuRoot *root, int index);

/* Lists the usable items (+0x1140, count +0x1684) and finds the chosen
   one's place (+0x1680). */
void func_08A18974(Screen1AB08 *self)
{
    Options29 *options = (Options29 *)((char *)D_08A23FF0->work + 0x652C);
    int count = func_088CC22C();
    int i;

    WORK_INT(self->work, 0x1680) = 0;
    WORK_INT(self->work, 0x1684) = 0;
    for (i = 0; i < count; i++) {
        if (func_088CC3B0(i) != 0 && func_088CC364(i) != 0) {
            (&WORK_SHORT(self->work, 0x1140))[WORK_INT(self->work, 0x1684)] = i;
            if (i == options->field_18) {
                WORK_INT(self->work, 0x1680) = WORK_INT(self->work, 0x1684);
            }
            WORK_INT(self->work, 0x1684)++;
        }
    }
}

/* Lists the root's entries (+0x1658, count +0x169C less one) and finds
   the chosen one's place plus one (+0x1698). */
void func_08A18A4C(Screen1AB08 *self)
{
    Options29 *options = (Options29 *)((char *)D_08A23FF0->work + 0x652C);
    int i;

    WORK_INT(self->work, 0x1698) = 0;
    WORK_INT(self->work, 0x169C) = func_08A20954(D_08A23FF0) + 1;
    for (i = 0; i < WORK_INT(self->work, 0x169C) - 1; i++) {
        (&WORK_INT(self->work, 0x1658))[i] = func_08A20960(D_08A23FF0, i);
        if (options->field_DA == (&WORK_INT(self->work, 0x1658))[i]) {
            WORK_INT(self->work, 0x1698) = i + 1;
        }
    }
}
