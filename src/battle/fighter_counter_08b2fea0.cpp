#include "battle/agent27_types.h"

extern "C" {

struct CounterArg {
    char pad00[0xF];
    unsigned char flags;    /* 0x0F */
};

class FighterCounter {
public:
    char pad000[0x544];
    AGENT27_ACTOR_VIRTUALS
};

int func_08B343D0(FighterCounter *fighter);
int func_088377C0(FighterCounter *fighter);

unsigned char func_08B2FEA0(FighterCounter *fighter, CounterArg *arg)
{
    if (func_08B343D0(fighter) != 0 && (arg->flags & 1) != 0) {
        int result = 0;
        if (fighter->slot_150() != 0 && func_088377C0(fighter) != 0) {
            result = 1;
        }
        return result;
    }
    return 0;
}

}
