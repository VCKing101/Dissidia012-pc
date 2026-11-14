#include "menu/agent20_types.h"

/* Moves the page by delta (clamped to 0..5) and starts the 10-frame scroll between the two offsets. */
extern int func_089F00C8(A20EntryOwner *self, unsigned short item);
extern void func_089F3B70(A20EntryOwner *self);

void func_089F397C(A20EntryOwner *self, short delta)
{
    int page = self->field_14C->field_2738 + delta;
    int from;
    if (page < 0) {
        page = 0;
    } else if (page > 5) {
        page = 5;
    }
    from = func_089F00C8(self, self->field_14C->field_2736);
    self->field_14C->field_3224 = from;
    self->field_14C->field_2738 = page;
    self->field_14C->field_3228 = func_089F00C8(self, self->field_14C->field_2736) - from;
    self->field_14C->field_322C = 10;
    self->field_14C->field_322E = 0;
    func_089F3B70(self);
}
