#include "menu/auto_region1.h"

/* ScreenE9D38: runs two steps on field_140, then func_089E7E7C(2). */
void func_089E8F5C(ScreenE9D38 *self)
{
    func_089EA6E8(self->field_140);
    func_089EA664(self->field_140);
    func_089E7E7C(self, 2);
}
