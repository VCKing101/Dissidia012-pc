#include "main/auto_region2.h"

/* func_0883E108 on the actor's link; 0 without one. */
int func_08833B2C(ActorView *self, int arg1, int arg2, int arg3)
{

    if (self->link == 0) {
        return 0;
    } else {
        return func_0883E108(self->link, arg1, arg2, arg3);
    }
}
