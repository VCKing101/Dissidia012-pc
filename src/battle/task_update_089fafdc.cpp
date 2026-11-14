#include "battle/agent11_types.h"

class Part089FAFDCTask : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x10];
    char part_1C[0x2C];
    char part_48[0x38];
    char part_80[0x48];
    char part_C8[0x4];
};

extern "C" {

extern void func_089F602C(void *part, float x);
extern void func_089F83AC(void *part);
extern int func_089FB354(Part089FAFDCTask *self, float x, float y);
extern void func_08A08334(void *part);
extern void func_08A10C88(void *part);

/* Runs the current state, then updates the task's parts. */
int func_089FAFDC(Part089FAFDCTask *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_089FB354(self, x, y);
    func_08A10C88(self->part_1C);
    func_089F83AC(self->part_48);
    func_089F602C(self->part_80, x);
    func_08A08334(self->part_C8);
    return 0;
}

}
