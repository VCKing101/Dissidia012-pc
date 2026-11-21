#include "menu/agent36_types.h"

/* Name list screen: runs the current step (+0x2A08) of its three-step
 * member-function table; true once the step is 3. The table is built in
 * place and called through the real member-function pointer type, which
 * gives SNC's own dispatch. */

extern "C" {
void func_08A3FB10(NameScreen36 *self);
void func_08A3FDD8(NameScreen36 *self);
void func_08A3E008(NameScreen36 *self);
}

extern "C" bool func_08A3E278(NameScreen36 *self)
{
    Step36 steps[3];

    steps[0].delta = 0;
    steps[0].index = 0;
    steps[0].target = (void *)func_08A3FB10;
    steps[1].delta = 0;
    steps[1].index = 0;
    steps[1].target = (void *)func_08A3FDD8;
    steps[2].delta = 0;
    steps[2].index = 0;
    steps[2].target = (void *)func_08A3E008;
    (((Stepper36 *)self)->*(*(StepFn36 *)&steps[self->work->field_2A08]))();
    return self->work->field_2A08 == 3;
}
