#include "menu/agent29_types.h"

/* Screen11510: shows the models and texts of the page in view
   (+0x113C; on the empty save only page 2) and hides the other two
   pages' (ranges in D_08A11DD6); with none in view, shows the frame
   models (0x51, 0x58, 0x59) and the frame text (0x5A; the texts run
   on past the region header's 0x58). */
typedef struct PageRange29 {
    unsigned short first_model;
    unsigned short last_model;
    unsigned short first_text;
    unsigned short last_text;
} PageRange29;

extern const PageRange29 D_08A11DD6[];

void func_08A0CA3C(Screen11510 *self)
{
    int shown = -1;
    int i;
    int j;
    int last;

    if (WORK_BYTE(self->work, 0x1110) != 0) {
        if (self->work->field_113C == 2) {
            shown = self->work->field_113C;
        }
    } else {
        shown = self->work->field_113C;
    }
    for (i = 0; i < 3; i++) {
        j = D_08A11DD6[i].first_model;
        last = D_08A11DD6[i].last_model;
        for (; j <= last; j++) {
            if (i == shown) {
                func_088D9CD8(self->work->handles[j]);
            } else {
                func_088D9CFC(self->work->handles[j]);
            }
        }
        j = D_08A11DD6[i].first_text;
        last = D_08A11DD6[i].last_text;
        for (; j <= last; j++) {
            if (self->work->texts[j] != 0) {
                if (i == shown) {
                    func_0887B924(self->work->texts[j]);
                } else {
                    func_0887B944(self->work->texts[j]);
                }
            }
        }
    }
    if (shown == -1) {
        func_088D9CD8(self->work->handles[0x51]);
        func_088D9CD8(self->work->handles[0x58]);
        func_088D9CD8(self->work->handles[0x59]);
        func_0887B924((void *)WORK_INT(self->work, 0x9E8));
    } else {
        func_0887B944((void *)WORK_INT(self->work, 0x9E8));
    }
}
