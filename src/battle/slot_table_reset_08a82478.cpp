#include "battle/agent14_types.h"

extern "C" void func_08A82478(SlotTable *self)
{
    self->active = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    for (int i = 0; i < 34; i++) {
        SlotEntry *e = &self->entries[i];
        e->used = 0;
        e->kind = 0;
        e->value = 0;
        e->time = 0.0f;
    }
    self->field_1A8 = 0;
    self->field_1AC = 0;
}
