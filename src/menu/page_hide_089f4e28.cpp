#include "menu/agent29_types.h"

/* Screen0B6A0: hides the page models 0x95..0xE5 (and resets their
   offsets) and hides (keep) or frees the page texts 0x16..0x22. */
extern "C" {
void func_088D9CFC(void *model);

void func_089F4E28(Screen0B6A0 *self, unsigned char keep)
{
    int i;

    for (i = 0x95; i < 0xE6; i++) {
        func_088D9CFC(self->work->handles[i]);
        MenuModel *model = self->work->handles[i];
        model->field_34 = 0;
        model->field_30 = 0;
        model->field_38 = 0;
    }
    for (i = 0x16; i < 0x23; i++) {
        void *text = self->work->texts[i];
        if (keep != 0) {
            if (text != 0) {
                func_0887B944(text);
            }
        } else if (text != 0) {
            func_0887BA5C(text);
            self->work->texts[i] = 0;
        }
    }
    self->work->field_30D0 = 0;
}
}
