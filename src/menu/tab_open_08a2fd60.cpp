#include "menu/agent36_types.h"

/* Tabbed pages: opens the page on the battle table's layout: plays its
 * scripts, remakes texts 0x14 and 0x15 and resets the page state. */
extern "C" {
extern ScriptCmd36 D_08A35824[];
extern ScriptCmd36 D_08A35838[];
extern ScriptCmd36 D_08A35860[];
}

extern "C" void func_08A2FD60(TabScreen36 *self)
{
    Model36 **handles = (Model36 **)D_08A457EC->table->handles;

    func_088D9CFC(handles[1]);
    func_088D3B88(self->work->layout, handles, D_08A35824, 0, 0x120);
    func_088D3CF0(self->work->layout, handles, D_08A35838);
    handles[0x2A]->field_0E = 3;
    handles[0x27]->field_0E = 1;
    if (self->work->texts[0x14] != 0) {
        func_0887BA5C(self->work->texts[0x14]);
        self->work->texts[0x14] = 0;
    }
    self->work->texts[0x14] = func_08A412A8(D_08A457EC, 0x15, 0, 0);
    if (self->work->texts[0x15] != 0) {
        func_0887BA5C(self->work->texts[0x15]);
        self->work->texts[0x15] = 0;
    }
    self->work->texts[0x15] = func_08A412A8(D_08A457EC, 0x10, 0, 0);
    func_088D428C(handles, self->work->texts, D_08A35860, 1);
    func_08A41940(D_08A457EC);
    self->work->field_6BC = 0;
    self->work->flags |= 0x500;
}
