#include "menu/agent29_types.h"

/* Screen15A88: places the cursor model (0x43) on the frame of the current
   tab (+0xCB8) and plays the selected or normal scripts of the three
   tabs (D_08A15F58). */
typedef struct TabScripts29 {
    const short *selected;
    const unsigned short *selected_texts;
    const short *normal;
    const unsigned short *normal_texts;
} TabScripts29;

extern const unsigned short D_08A15F88[];
extern const TabScripts29 D_08A15F58[];
extern void func_088D9AF4(void *model, unsigned int *color);

void func_08A12034(Screen15A88 *self)
{
    float matrix[16];
    unsigned int color;
    int i;

    func_088DA0A8(self->work->handles[0x3F], D_08A15F88[WORK_INT(self->work, 0xCB8)], matrix, &color);
    func_088D9B84(self->work->handles[0x43], matrix);
    func_088D9AF4(self->work->handles[0x43], &color);
    for (i = 0; i < 3; i++) {
        if (i == WORK_INT(self->work, 0xCB8)) {
            func_088D3F00(&self->work->layout, self->work->handles, D_08A15F58[i].selected);
            func_088D44EC(self->work->handles, self->work->texts, D_08A15F58[i].selected_texts);
        } else {
            func_088D3F00(&self->work->layout, self->work->handles, D_08A15F58[i].normal);
            func_088D44EC(self->work->handles, self->work->texts, D_08A15F58[i].normal_texts);
        }
    }
}
