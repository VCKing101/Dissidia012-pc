#include "battle/auto_region3.h"

/* Part815C8 reset. */
void func_08A80800(Part815C8 *self)
{
    self->active = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    self->field_10 = 0;
    self->field_11 = 0;
    self->field_14 = 0;
    self->field_54 = -1;
    self->field_58 = 0;
    self->field_5C = 0;
    func_089A34E0(&self->field_18);
    func_089A34E0(&self->field_48);
    func_089A34E0(&self->field_24);
    func_089A34E0(&self->field_30);
    func_089A34E0(&self->field_3C);
    self->field_60 = 0;
    self->field_61 = 0;
    self->field_64 = 0;
    self->field_68 = 0;
}
