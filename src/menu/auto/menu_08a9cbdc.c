#include "menu/auto_region3.h"

/* ScreenA3F44 constructor: the task (id 0x11187), field_158 and field_894. */
ScreenA3F44 *func_08A9CBDC(ScreenA3F44 *self)
{
    func_08864FF0(&self->task, 1, 0x11187, 0x4C);
    func_0891A810(self->field_158);
    func_08AB41E0(&self->field_894);
    return self;
}
