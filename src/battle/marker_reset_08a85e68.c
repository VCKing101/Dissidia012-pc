#include "battle/agent14_types.h"

extern void func_089A3890(Vec4 *v);

int func_08A85E68(Marker *self)
{
    int none = -1;

    if (self->active == 0) {
        return none;
    }
    self->field_08 = 0;
    self->field_0C = 0;
    func_089A3890(&self->position);
    self->id = none;
    return 0;
}
