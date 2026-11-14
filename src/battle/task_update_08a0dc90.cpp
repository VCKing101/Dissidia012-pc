#include "battle/agent11_types.h"

class Part08A0DC90Task : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0xC];
    char part_18[0x28];
    char part_40[0x30];
    char part_70[0x24];
    char part_94[0x4];
};

extern "C" {

extern void func_08A01948(void *part);
extern void func_08A01F94(void *part);
extern void func_08A0D814(void *part);
extern int func_08A0DF54(Part08A0DC90Task *self, float x, float y);
extern void func_08A169DC(void *part);

/* Runs the current state, then updates the task's parts. */
int func_08A0DC90(Part08A0DC90Task *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A0DF54(self, x, y);
    func_08A01948(self->part_18);
    func_08A169DC(self->part_40);
    func_08A01F94(self->part_70);
    func_08A0D814(self->part_94);
    return 0;
}

}
