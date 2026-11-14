#include "battle/agent11_types.h"

class Part08A2A844Task : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x10];
    char part_1C[0x2C];
    char part_48[0x38];
    char part_80[0x4];
};

extern "C" {

extern void func_089F602C(void *part, float x);
extern void func_089F83AC(void *part);
extern void func_08A10C88(void *part);
extern int func_08A2AAE4(Part08A2A844Task *self, float x, float y);

/* Runs the current state, then updates the task's parts. */
int func_08A2A844(Part08A2A844Task *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A2AAE4(self, x, y);
    func_08A10C88(self->part_1C);
    func_089F83AC(self->part_48);
    func_089F602C(self->part_80, x);
    return 0;
}

}
