#include "menu/agent36_types.h"

/* Menu root: plays the current mode's layout and text scripts, then its
 * extras. */
void func_08A1EABC(MenuRoot36 *self, void *layout, void *handles, void *texts)
{
    func_088D3F00(layout, handles, D_08A23DB4[self->work->mode].layout_script);
    func_088D44EC(handles, texts, D_08A23DB4[self->work->mode].text_script);
    if (D_08A23DB4[self->work->mode].text == 1) {
        func_08A1EA3C(self, handles);
    }
    if (D_08A23DB4[self->work->mode].extra != 0) {
        func_08A1E6B4(self, 0, layout, handles, texts, self->work->field_25BBC);
    }
    func_08A1E168(self);
}
