#include "menu/agent29_types.h"

/* Screen1AB08: plays the selected or normal scripts of tab `index`
   (D_08A1B13C), selected when it is the current tab (+0x16B4). */
typedef struct TabScripts29 {
    const short *selected;
    const unsigned short *selected_texts;
    const short *normal;
    const unsigned short *normal_texts;
} TabScripts29;

extern const TabScripts29 D_08A1B13C[];

void func_08A18C34(Screen1AB08 *self, int index)
{
    if (WORK_INT(self->work, 0x16B4) == index) {
        func_088D3F00(&self->work->layout, self->work->handles, D_08A1B13C[index].selected);
        func_088D44EC(self->work->handles, self->work->texts, D_08A1B13C[index].selected_texts);
    } else {
        func_088D3F00(&self->work->layout, self->work->handles, D_08A1B13C[index].normal);
        func_088D44EC(self->work->handles, self->work->texts, D_08A1B13C[index].normal_texts);
    }
}
