#include "menu/agent36_types.h"

/* Grid page: opens the page: plays its layout scripts, remakes text 9,
 * sets up the cursor and the page state. */
extern "C" {
extern ScriptCmd36 D_08A2E642[];
extern ScriptCmd36 D_08A2E65E[];
extern ScriptCmd36 D_08A2E696[];
}

struct CursorFlags {
    static void Show(Cursor36 *cursor) { cursor->flags |= 1; }
};

extern "C" void func_08A2B200(GridScreen36 *self)
{
    Model36 **handles = (Model36 **)self->work->handles;
    unsigned char on;

    func_088D3B88(self->work->layout, handles, D_08A2E642, 0, 0x120);
    func_088D3CF0(self->work->layout, handles, D_08A2E65E);
    handles[0x41]->field_0E = 4;
    if (self->work->texts[9] != 0) {
        func_0887BA5C(self->work->texts[9]);
        self->work->texts[9] = 0;
    }
    self->work->texts[9] = func_08A2BA30(self, 5, 0, 0);
    {
        void **texts = self->work->texts;

        on = 1;
        func_088D428C(handles, texts, D_08A2E696, 1);
    }
    func_088D46E4(&self->work->cursor, 0, 0, 1, 0, -1);
    CursorFlags::Show(&self->work->cursor);
    func_08A2B328(self);
    self->work->field_1514 = 0;
    self->work->field_1558 = on;
    self->work->field_14F8 |= 0xA;
}
