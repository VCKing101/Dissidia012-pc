#include "battle/agent28_types.h"

/* Runs the handler of the battle phase's current state. */
extern int func_089F0600(PhaseState *phase);
extern int func_089F0658(PhaseState *phase);
extern int func_089F06E8(PhaseState *phase);
extern int func_089F0720(PhaseState *phase);
extern int func_089F08EC(PhaseState *phase);
extern int func_089F0990(PhaseState *phase);
extern int func_089F09FC(PhaseState *phase);
extern int func_089F0A44(PhaseState *phase);
extern int func_089F0AE8(PhaseState *phase);
extern int func_089F0B38(PhaseState *phase);
extern int func_089F0B80(PhaseState *phase);
extern int func_089F0C24(PhaseState *phase);

int func_089F0500(PhaseState *phase)
{
    switch (phase->state) {
    case 0:
        return func_089F0600(phase);
    case 1:
        return func_089F0658(phase);
    case 2:
        return func_089F06E8(phase);
    case 3:
        return func_089F0720(phase);
    case 4:
        return func_089F08EC(phase);
    case 5:
        return func_089F0990(phase);
    case 6:
        return func_089F09FC(phase);
    case 7:
        return func_089F0A44(phase);
    case 8:
        return func_089F0AE8(phase);
    case 9:
        return func_089F0B38(phase);
    case 10:
        return func_089F0B80(phase);
    case 11:
        return func_089F0C24(phase);
    }
    return 0;
}
