#include "menu/agent36_types.h"

/* Tabbed pages: opens the list view of the current page: plays its
 * scripts, picks the other list's entry for func_08A41980 and opens the
 * list (func_08A2F430). */

extern ScriptCmd36 D_08A35580[];
extern ScriptCmd36 D_08A3561C[];

void func_08A30688(TabScreen36 *self)
{
    void **handles = D_08A457EC->table->handles;
    ListPage36 *page;

    self->work->flags = 0;
    self->work->field_6D3 = 0;
    func_088D3B88(self->work->layout, handles, D_08A35580, 0, 0x120);
    func_088D3CF0(self->work->layout, handles, D_08A3561C);
    func_088D9F5C(handles[2], -1, 1);
    func_088D9F5C(handles[1], 0, 1);
    func_088D9CFC(handles[1]);
    func_08A2EB10(self);
    page = &D_08A35878[self->work->tab];
    if (self->work->counts[page->list ^ 1] == 0) {
        func_08A41980(D_08A457EC, -1);
    } else {
        func_08A41980(D_08A457EC, page->field_14);
    }
    func_08A2F430(self, page);
    self->work->flags |= 0xC0;
    self->work->field_6D0 = 0;
    self->work->field_6A8 = 3;
}
