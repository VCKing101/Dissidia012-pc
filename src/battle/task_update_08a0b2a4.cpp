#include "battle/agent11_types.h"

class Part08A0B2A4Task : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x8];
    char part_14[0x2C];
    char part_40[0x28];
    char part_68[0x30];
    char part_98[0x4];
};

extern "C" {

extern void func_08A01948(void *part);
extern void func_08A08334(void *part);
extern int func_08A0B748(Part08A0B2A4Task *self, float x, float y);
extern void func_08A10C88(void *part);
extern void func_08A169DC(void *part);

/* Runs the current state, then updates the task's parts. */
int func_08A0B2A4(Part08A0B2A4Task *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A0B748(self, x, y);
    func_08A10C88(self->part_14);
    func_08A01948(self->part_40);
    func_08A169DC(self->part_68);
    func_08A08334(self->part_98);
    return 0;
}

}
