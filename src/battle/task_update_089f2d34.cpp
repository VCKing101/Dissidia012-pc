#include "battle/agent11_types.h"

class Part089F2D34Task : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x10];
    char part_1C[0x2C];
    char part_48[0x38];
    char part_80[0x170];
    char part_1F0[0x4];
};

extern "C" {

extern void func_089F249C(void *part);
extern int func_089F30AC(Part089F2D34Task *self, float x, float y);
extern void func_089F83AC(void *part);
extern void func_08A08334(void *part);
extern void func_08A10C88(void *part);

/* Runs the current state, then updates the task's parts. */
int func_089F2D34(Part089F2D34Task *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_089F30AC(self, x, y);
    func_08A10C88(self->part_1C);
    func_089F83AC(self->part_48);
    func_089F249C(self->part_80);
    func_08A08334(self->part_1F0);
    return 0;
}

}
