#include "battle/agent11_types.h"

class Part08A27E1CTask : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x10];
    char part_1C[0x28];
    char part_44[0x30];
    char part_74[0x34];
    char part_A8[0x4];
};

extern "C" {

extern void func_089FC7E0(void *part);
extern void func_089FE054(void *part);
extern void func_08A01948(void *part);
extern void func_08A169DC(void *part);
extern int func_08A28070(Part08A27E1CTask *self, float x, float y);

/* Runs the current state, then updates the task's parts. */
int func_08A27E1C(Part08A27E1CTask *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A28070(self, x, y);
    func_08A01948(self->part_1C);
    func_08A169DC(self->part_44);
    func_089FE054(self->part_74);
    func_089FC7E0(self->part_A8);
    return 0;
}

}
