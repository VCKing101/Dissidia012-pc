/* Marks the round active and running, then enters mode 8. */
#include "main/agent17_types.h"

void func_088AF75C(Round *round, int mode);
void func_088AF134(Round *round, int mode);

void func_088AF3E0(Round *round)
{
    round->flags.bits.b1 = 1;
    round->flags.bits.b0 = 1;
    func_088AF75C(round, 8);
    func_088AF134(round, 1);
}
