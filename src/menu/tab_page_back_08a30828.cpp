#include "menu/agent36_types.h"

/* Tabbed pages: back from the list view to the page (with its message box
 * when +0x6D4 is set). */
extern "C" {
extern ScriptCmd36 D_08A355F8[];
extern ScriptCmd36 D_08A3567C[];
}

struct MessageBox36 {
    static void Open(int id) {
        int kind = 0x11;
        int none = 0;
        func_088854E8(D_089D69E8, kind, (unsigned short)id);
        func_088853A8(D_089D69E8, 1);
        func_08885890(D_089D69E8, none);
    }
};

extern "C" void func_08A30828(TabScreen36 *self)
{
    ListPage36 *page;

    func_08A419A4(D_08A457EC);
    page = &D_08A35878[self->work->tab];
    if (self->work->field_6D4 != 0) {
        MessageBox36::Open((unsigned short)page->field_08);
    }
    func_08A2FA6C(self, page);
    func_088D3B88(self->work->layout, self->work->handles, D_08A355F8, 0, 0x120);
    func_088D3CF0(self->work->layout, self->work->handles, D_08A3567C);
    func_08A327CC(self);
    self->work->flags = 0;
    self->work->field_6A8 = 6;
    self->work->field_6D4 = 0;
    self->work->flags |= 0x10;
}
