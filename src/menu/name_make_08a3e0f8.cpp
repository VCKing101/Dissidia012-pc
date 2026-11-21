#include "menu/agent36_types.h"

/* Constructor of the name list screen: base, vtable and the work block on
 * the battle table's layout, cleared. */

extern "C" {
extern char D_08A40248[];
void func_08A408A0(void *self);
void func_08A41454(BattleGlobal36 *battle, void *block);
}

/* The name list screen object (vtable D_08A40248). */
struct NameObject36 {
    void *vtable;
    NameWork36 block;         /* +0x0004 */
    NameWork36 *work;         /* +0x2A14 */
};

extern "C" NameObject36 *func_08A3E0F8(NameObject36 *self)
{
    int i;

    func_08A408A0(self);
    self->vtable = D_08A40248;
    self->work = &self->block;
    self->work->layout = (void **)D_08A457EC->table;
    self->work->handles = (Model36 **)D_08A457EC->table->handles;
    self->work->messages = D_08A457EC->table->messages;
    for (i = 0; i < 0x20; i++) {
        self->work->texts[i] = 0;
    }
    func_08935828(&self->work->all, 0, sizeof(NameRecord36));
    func_08935828(self->work->records, 0, sizeof(self->work->records));
    func_08935828(self->work->entries, 0, sizeof(self->work->entries));
    self->work->field_2A04 = 0;
    self->work->field_2A0C = 0;
    self->work->record_count = 0;
    self->work->field_29FC = 0;
    self->work->count = 0;
    self->work->field_2A0D = 0;
    {
        void *block = self->work->field_114;
        func_08A41454(D_08A457EC, block);
    }
    self->work->field_2A08 = 0;
    return self;
}
