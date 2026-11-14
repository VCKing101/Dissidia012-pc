#include "battle/agent11_types.h"

class Part08A1C96CTask : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x8];
    char part_14[0x28];
    char part_3C[0x28];
    char part_64[0x28];
    char part_8C[0x38];
    char part_C4[0x2C];
    char part_F0[0x4];
};

extern "C" {

extern void func_089F83AC(void *part);
extern void func_08A01948(void *part);
extern void func_08A08334(void *part);
extern void func_08A10C88(void *part);
extern int func_08A1CDF0(Part08A1C96CTask *self, float x, float y);

/* Runs the current state, then updates the task's parts. */
int func_08A1C96C(Part08A1C96CTask *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A1CDF0(self, x, y);
    func_08A01948(self->part_14);
    func_08A01948(self->part_3C);
    func_08A01948(self->part_64);
    func_089F83AC(self->part_8C);
    func_08A10C88(self->part_C4);
    func_08A08334(self->part_F0);
    return 0;
}

}
