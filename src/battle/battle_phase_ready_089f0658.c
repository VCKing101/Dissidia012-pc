#include "battle/agent28_types.h"

/* State 1: once the phase is ready, starts the cue icon, hands the next
 * turn to the root and moves to state 2. */
extern RootHolder *D_089D6A10;
extern int func_089EF4F0(void *cue);
extern void func_089DDB50(void *root, int side, int flags);

int func_089F0658(PhaseState *phase)
{
    if (phase->ready == 1) {
        void *root;

        func_089EF4F0(phase->cue);
        root = D_089D6A10->root;
        if (root == 0) {
            return -1;
        }
        func_089DDB50(root, (phase->wins + phase->losses + 1) % 2, 0);
        phase->state = 2;
    }
    return 0;
}
