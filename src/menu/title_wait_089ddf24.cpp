#include "menu/agent29_types.h"

/* The title screen's wait: once the start model (+0x1C of the state
   block at +0x1DC) is in state 2 and done, and the save check (when one
   runs) has finished with 1, starts the menu, frees the check and moves
   on; otherwise replays the model's motion 2 after 2,000,000 ticks. */
typedef struct TitleState29 {
    char pad_00[0x8];
    int step;
    char pad_0C[0x4];
    unsigned int shown; /* tick of the last motion */
    char pad_14[0x4];
    void *check;        /* +0x18 */
    MenuModel *model;   /* +0x1C */
    char pad_20[0x48];
    int checking;       /* +0x68 */
} TitleState29;

typedef struct Title29 {
    char pad_000[0x194];
    char menu[0x48];
    TitleState29 *state; /* +0x1DC */
} Title29;

extern "C" {
int func_0889F980(void *menu);
void func_088DAB44(void *object);
unsigned int func_089C5FB8(void);
}

/* Retail keeps the menu pointer in the frame across func_0889F980;
   kept[] stands for that slot. */
struct TitleModel29 {
    static bool Done(MenuModel *model) { return (model->field_20 & 0x20) != 0; }
};

extern "C" void func_089DDF24(Title29 *self)
{
    void *menu;
    void *kept[1];
    int result;
    int checking;

    if (self->state->model->field_0C == 2) {
        if (TitleModel29::Done(self->state->model)) {
            checking = self->state->checking;
            menu = self->menu;
            if (checking == 0
                || (kept[0] = menu, result = func_0889F980(menu), menu = kept[0], result == 1)) {
                func_0889F704(menu);
                if (self->state->check != 0) {
                    func_088DAB44(self->state->check);
                    self->state->check = 0;
                }
                self->state->step++;
            }
        }
    } else if (func_089C5FB8() - self->state->shown >= 2000000) {
        func_088D9F5C(self->state->model, 2, 1);
    }
}
