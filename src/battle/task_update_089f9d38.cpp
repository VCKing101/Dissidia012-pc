#include "battle/agent11_types.h"

class Part089F9D38Task : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x8];
    char part_14[0x28];
    char part_3C[0x30];
    char part_6C[0x24];
    char part_90[0x3C];
    char part_CC[0x4];
};

extern "C" {

extern int func_089F9FCC(Part089F9D38Task *self, float x, float y);
extern void func_08A008FC(void *part);
extern void func_08A01948(void *part);
extern void func_08A01F94(void *part);
extern void func_08A08334(void *part);
extern void func_08A169DC(void *part);

/* Runs the current state, then updates the task's parts. */
int func_089F9D38(Part089F9D38Task *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_089F9FCC(self, x, y);
    func_08A01948(self->part_14);
    func_08A169DC(self->part_3C);
    func_08A01F94(self->part_6C);
    func_08A008FC(self->part_90);
    func_08A08334(self->part_CC);
    return 0;
}

}
