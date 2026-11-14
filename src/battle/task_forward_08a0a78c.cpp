#include "battle/agent11_types.h"

class BattleTaskA78C : public BattleTask {
public:
    char pad_014[0x2C];
    char field_40[0x4];
};

extern "C" {

extern void func_089F652C(void *sub);

int func_08A0A78C(BattleTaskA78C *self)
{
    if (!self->isActive()) {
        return -1;
    }
    func_089F652C(self->field_40);
    return 0;
}

}
