#include "menu/agent36_types.h"

/* Menu root: closes the current pose once both of its models (handles
 * 0x26 and 0x25) have stopped. Returns 0 while they still move, 1 when
 * there was no pose or it is now closed. */
extern "C" int func_08A20180(MenuRoot36 *self, void *layout, Model36 **handles, void **texts)
{
    int kind = self->work->pose_kind;
    int none = -1;

    if (kind == none) {
        return 1;
    }
    if (!Flags36::Has(handles[0x26]->field_20, 0x20) ||
        !Flags36::Has(handles[0x25]->field_20, 0x20)) {
        return 0;
    }
    func_088D3EA0(layout, handles, D_08A23FB0[kind].close_script);
    Model36 *model = handles[0x27];
    model->field_34 = 0;
    model->field_30 = 0;
    model->field_38 = 0;
    if (texts[0x1A] != 0) {
        func_0887BA5C(texts[0x1A]);
        texts[0x1A] = 0;
    }
    self->work->pose_kind = none;
    return 1;
}
