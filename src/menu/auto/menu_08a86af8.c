#include "menu/auto_region3.h"

/* Screen87EAC: frees the heap and field_134. */
void func_08A86AF8(Screen87EAC *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
    func_0888208C(self->field_134);
    self->field_134 = 0;
}
