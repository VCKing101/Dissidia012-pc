#include "battle/agent11_types.h"

class Part08A0A700Task : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x8];
    char part_14[0x2C];
    char part_40[0x938];
    char part_978[0x4];
};

extern "C" {

extern void func_089F64F8(void *part);
extern void func_08A08334(void *part);
extern int func_08A0AAD4(Part08A0A700Task *self, float x, float y);
extern void func_08A10C88(void *part);

/* Runs the current state, then updates the task's parts. */
int func_08A0A700(Part08A0A700Task *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A0AAD4(self, x, y);
    func_08A10C88(self->part_14);
    func_089F64F8(self->part_40);
    func_08A08334(self->part_978);
    return 0;
}

}
