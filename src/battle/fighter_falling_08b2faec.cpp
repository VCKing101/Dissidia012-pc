#include "battle/agent27_types.h"

extern "C" {

extern float D_08B3ABCC;

class FighterFall {
public:
    char pad000[0x1A8];
    float fallSpeed;        /* 0x1A8 */
    char pad1AC[0x398];
    AGENT27_ACTOR_VIRTUALS
};

int func_08837310(FighterFall *fighter);

struct FighterFallOps {
    static unsigned char falling(FighterFall *fighter)
    {
        int result = 0;
        if (fighter->slot_150() != 0 || (func_08837310(fighter) != 0 && fighter->fallSpeed < D_08B3ABCC)) {
            result = 1;
        }
        return result;
    }
};

unsigned char func_08B2FAEC(FighterFall *fighter)
{
    return FighterFallOps::falling(fighter);
}

}
