#include "menu/agent29_types.h"

/* ScreenE75C4: when a gate is selected (+0xB20) and it has a light,
   copies the light's matrix and colour from model handles[0x41] (frame
   D_089E75C0[+0xB2C]) onto handles[0x3E]. */
extern unsigned short D_089E75C0[];
extern void func_088D9AF4(void *model, unsigned int *color);

void func_089E343C(ScreenE75C4 *self, MenuModel **handles)
{
    float matrix[16];
    unsigned int color;
    int gate = self->work->field_B20;

    if (gate == -1) {
        return;
    }
    if (D_089E7434[gate].light == 0xFFFF) {
        return;
    }
    func_088DA0A8(handles[0x41], D_089E75C0[WORK_INT(self->work, 0xB2C)], matrix, &color);
    func_088D9B84(handles[0x3E], matrix);
    func_088D9AF4(handles[0x3E], &color);
}
