#include "battle/agent14_types.h"

extern void func_08A85D14(Marker *self);
extern int func_08A85E68(Marker *self);

int func_08A85D30(Marker *self, void *owner)
{
    func_08A85D14(self);
    if (owner == 0) {
        return -1;
    }
    self->owner = owner;
    self->active = 1;
    if (func_08A85E68(self) != 0) {
        func_08A85D14(self);
        return -2;
    }
    return 0;
}
