#include "menu/agent36_types.h"

/* Screen28560: builds the row request (id and, for a named entry, its
 * name record) and hands it to the request queue. */

void func_08A24A60(Screen28560x *self)
{
    self->work->request.id = -1;
    self->work->request.name = 0;
    if (self->work->flags & 2) {
        if (self->work->entries[self->work->cursor.index].flags & 1) {
            int found;
            self->work->request.id = self->work->rows[self->work->row].id;
            {
                int slot = self->work->rows[self->work->row].slot;
                NameSlot36 *names = D_089D712C->names;
                NameSlot36 *name = &names[slot];

                found = func_088C8158(&self->work->ref, name, self->work->entries[self->work->cursor.index].arg);
            }
            if (found != 0) {
                self->work->request.name = &self->work->ref;
            }
        }
    } else if (self->work->field_25A8 != 0) {
        self->work->request.id = self->work->rows[self->work->row].id;
    }
    func_08AF55B8(D_08B01CF8, &self->work->request);
}
