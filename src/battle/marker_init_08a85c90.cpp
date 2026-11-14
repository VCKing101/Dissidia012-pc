#include "battle/agent14_types.h"

extern "C" {

extern void func_089A3890(Vec4 *v);
extern void func_089A34E0(Vec3 *v);

void func_08A85C90(Marker *self)
{
    self->active = 0;
    self->owner = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    func_089A3890(&self->position);
    self->id = -1;
    func_089A34E0(&self->field_24);
    func_089A34E0(&self->field_30);
    for (int i = 0; i < 2; i++) {
        MarkerPair *pair = &self->pairs[i];
        pair->first = -1;
        pair->second = -1;
    }
    self->field_4C = 0;
    func_089A34E0(&self->field_50);
}

}
