#include "menu/agent36_types.h"

/* Screen28560: fills the item panel request (item, its picture, or the
 * named record's picture for a named entry) and queues it. */
extern "C" void func_08A25730(Screen28560x *self)
{
    self->work->panel.handles = self->work->handles;
    self->work->panel.texts = self->work->texts;
    self->work->panel.messages = D_08A23FF0->work->messages;
    self->work->panel.field_10 = 0;
    self->work->panel.x = 0x62;
    self->work->panel.y = 0x91;
    self->work->panel.item = func_08A256C8(self);
    ItemRef36 ref = func_088B97E8(self->work->panel.item);
    self->work->panel.field_FC = func_088C1998(&ref);
    if (self->work->entries[self->work->cursor.index].flags & 1) {
        int slot = self->work->rows[self->work->row].slot;
        NameSlot36 *names = D_089D712C->names;
        NameSlot36 *name = &names[slot];

        if (func_088C8158(&self->work->ref, name, self->work->entries[self->work->cursor.index].arg) != 0) {
            self->work->panel.field_FC = func_088C1998(&self->work->ref);
        }
    }
    func_08AFF218(D_08B01CF8, &self->work->panel);
}
