#include "battle/agent11_types.h"

class Part08A20F0CTask : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x8];
    char part_14[0x28];
    char part_3C[0x30];
    char part_6C[0x24];
    char part_90[0x38];
    char part_C8[0x4];
};

extern "C" {

extern void func_08A01948(void *part);
extern void func_08A01F94(void *part);
extern void func_08A169DC(void *part);
extern void func_08A202DC(void *part);
extern void func_08A20850(void *part);
extern int func_08A21168(Part08A20F0CTask *self, float x, float y);

/* Runs the current state, then updates the task's parts. */
int func_08A20F0C(Part08A20F0CTask *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A202DC(self->part_90);
    func_08A20850(self->part_C8);
    func_08A21168(self, x, y);
    func_08A01948(self->part_14);
    func_08A169DC(self->part_3C);
    func_08A01F94(self->part_6C);
    return 0;
}

}
