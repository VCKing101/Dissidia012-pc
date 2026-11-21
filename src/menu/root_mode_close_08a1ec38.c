#include "menu/agent36_types.h"

/* Menu root: plays the current mode's closing layout script, clears handle
 * 0x1A, frees text 0x12 and releases the mode's extras. */
void func_08A1EC38(MenuRoot36 *self, void *layout, Model36 **handles, void **texts)
{
    Model36 *model;

    func_088D3EA0(layout, handles, D_08A23DB4[self->work->mode].layout_script);
    model = handles[0x1A];
    model->field_34 = 0;
    model->field_30 = 0;
    model->field_38 = 0;
    if (texts[0x12] != 0) {
        func_0887BA5C(texts[0x12]);
        texts[0x12] = 0;
    }
    if (D_08A23DB4[self->work->mode].extra != 0) {
        func_08A1E5D4(self, 0, layout, handles, texts, self->work->field_25BBC);
    }
}
