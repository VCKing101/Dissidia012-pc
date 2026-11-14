#include "main/auto_region1.h"

/* Passes self and arg to func_0880AD28 on the actor's link bit sets. */
void func_08808190(void *self, Actor *actor, int arg)
{
    int result;

    func_0880AD28(&actor->link->bits, self, arg, &result);
}
