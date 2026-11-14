#include "main/auto_region2.h"

/* Distance between the two actors' positions (func_089A3C18; the float stays in $f0). */
void func_08837AA4(ActorView *self, ActorView *other)
{
    func_089A3C18(self->position, other->position);
}
