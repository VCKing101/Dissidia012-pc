#include "battle/auto_region4.h"

/* PartBC608 setup: stores the owner, clears the position and resets. */
void func_08ABC608(PartBC608 *self, void *owner)
{
    self->owner = owner;
    func_089A34E0(&self->field_08);
    func_08ABC6B0(self);
}
