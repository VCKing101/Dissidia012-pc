#include "battle/agent11_types.h"

class Part08A09248Task : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x8];
    char part_14[0x28];
    char part_3C[0x30];
    char part_6C[0x30];
    char part_9C[0x4];
};

extern "C" {

extern void func_08A01948(void *part);
extern void func_08A08584(void *part);
extern void func_08A08AEC(void *part);
extern int func_08A09498(Part08A09248Task *self, float x, float y);
extern void func_08A169DC(void *part);

/* Runs the current state, then updates the task's parts. */
int func_08A09248(Part08A09248Task *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A09498(self, x, y);
    func_08A01948(self->part_14);
    func_08A169DC(self->part_3C);
    func_08A08AEC(self->part_6C);
    func_08A08584(self->part_9C);
    return 0;
}

}
