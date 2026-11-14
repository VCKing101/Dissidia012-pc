#include "menu/agent29_types.h"

/* Screen15A88: plays the selected or normal scripts of the three tabs
   (D_08A15F58) and shows the current tab's caption (text 0x32 + 2 * i)
   or hides the others, then shows the current tab's tip and places the
   cursor. */
typedef struct TabScripts29 {
    const unsigned short *selected;
    const unsigned short *selected_texts;
    const unsigned short *normal;
    const unsigned short *normal_texts;
} TabScripts29;

extern const TabScripts29 D_08A15F58[];
extern void func_08A1E1C8(MenuRoot *root, int row, int column);

void func_08A11F10(Screen15A88 *self)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (i == WORK_INT(self->work, 0xCB8)) {
            func_088D3CF0(&self->work->layout, self->work->handles, D_08A15F58[i].selected);
            func_088D428C(self->work->handles, self->work->texts, D_08A15F58[i].selected_texts, 1);
            func_0887B924(self->work->texts[0x32 + i * 2]);
        } else {
            func_088D3CF0(&self->work->layout, self->work->handles, D_08A15F58[i].normal);
            func_088D428C(self->work->handles, self->work->texts, D_08A15F58[i].normal_texts, 1);
            func_0887B944(self->work->texts[0x32 + i * 2]);
        }
    }
    func_08A1E1C8(D_08A23FF0, WORK_INT(self->work, 0xCB8) + 8, 0);
    func_08A12034(self);
}
