#include "battle/agent28_types.h"

/* Clears the bonus prize: drops both digit models and forgets the motion. */
extern "C" {

extern void func_088DA060(PrizeModel *model);

int func_08A191E8(BonusPrize *prize)
{
    int none = -1;
    if (!prize->open) {
        return none;
    }
    for (int i = 0; i < 2; i++) {
        if (prize->models[i] != 0) {
            func_088DA060(prize->models[i]);
            prize->models[i] = 0;
        }
    }
    prize->shown = 0;
    prize->motion = none;
    prize->bones = 0;
    return 0;
}

}
