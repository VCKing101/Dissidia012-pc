#include "battle/agent11_types.h"

struct Wheel { char data[0x118]; };

class WheelTask : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x4];
    Wheel wheels[2];
};

extern "C" {

extern int func_089FAA6C(WheelTask *self, float x, float y);
extern void func_08A17808(Wheel *wheel, float x);

/* Runs the current state, then turns both wheels by x. */
int func_089FA6DC(WheelTask *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_089FAA6C(self, x, y);
    for (int i = 0; i < 2; i++) {
        func_08A17808(&self->wheels[i], x);
    }
    return 0;
}

}
