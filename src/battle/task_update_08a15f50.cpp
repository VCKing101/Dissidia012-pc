#include "battle/agent11_types.h"

class Part08A15F50Task : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0xC];
    char part_18[0x2C];
    char part_44[0x38];
    char part_7C[0x98];
    char part_114[0x4];
};

extern "C" {

extern void func_089F83AC(void *part);
extern void func_08A08334(void *part);
extern void func_08A0E898(void *part);
extern void func_08A10C88(void *part);
extern int func_08A162C8(Part08A15F50Task *self, float x, float y);

/* Runs the current state, then updates the task's parts. */
int func_08A15F50(Part08A15F50Task *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A162C8(self, x, y);
    func_08A10C88(self->part_18);
    func_089F83AC(self->part_44);
    func_08A0E898(self->part_7C);
    func_08A08334(self->part_114);
    return 0;
}

}
