#include "battle/agent28_types.h"

/* Waits out the phase delay, then hands the next turn to the root and ends
 * the phase. */
extern RootHolder *D_089D6A10;
extern float D_089F0D28;
extern void func_089DDB50(void *root, int side, int flags);

int func_089F0B80(PhaseState *phase, float step)
{
    phase->time += step;
    if (phase->time >= D_089F0D28) {
        int ended = -1;
        void *root = D_089D6A10->root;
        if (root == 0) {
            return ended;
        }
        func_089DDB50(root, (phase->wins + phase->losses + 1) % 2, 0);
        phase->state = ended;
        phase->time = 0.0f;
    }
    return 0;
}
