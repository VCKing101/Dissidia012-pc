#include "battle/agent11_types.h"

extern "C" {

extern BattleFighter *func_089E95B0(unsigned int index);
extern int func_08842F28(int value, int limit);

int func_089E9AA8(unsigned int index, int value)
{
    if (index >= 2) {
        return 0;
    }
    BattleFighter *fighter = func_089E95B0(index);
    if (fighter == 0) {
        return 0;
    }
    return func_08842F28(value, fighter->slot_198());
}

}
