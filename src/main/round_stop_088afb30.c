/* Clears round flags 2 and 0, then leaves mode 0. */
#include "main/agent17_types.h"

void func_088AF134(Round *round, int mode);

void func_088AFB30(Round *round)
{
    round->flags.bits.b2 = 0;
    round->flags.bits.b0 = 0;
    func_088AF134(round, 0);
}
