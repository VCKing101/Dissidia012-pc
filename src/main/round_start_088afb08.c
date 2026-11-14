/* Sets round flags 4 and 7, then starts the round. */
#include "main/agent17_types.h"

void func_088AF3E0(Round *round);

void func_088AFB08(Round *round)
{
    round->flags.bits.b4 = 1;
    round->flags.bits.b7 = 1;
    func_088AF3E0(round);
}
