#include "battle/agent11_types.h"

class Part08A1A978Task : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x10];
    char part_1C[0x28];
    char part_44[0x24];
    char part_68[0x24];
    char part_8C[0x2C];
    char part_B8[0x38];
    char part_F0[0x48];
    char part_138[0x4];
};

extern "C" {

extern void func_089F602C(void *part, float x);
extern void func_089F83AC(void *part);
extern void func_08A01948(void *part);
extern void func_08A01F94(void *part);
extern void func_08A08334(void *part);
extern void func_08A10C88(void *part);
extern void func_08A18F00(void *part);
extern int func_08A1AE08(Part08A1A978Task *self, float x, float y);

/* Runs the current state, then updates the task's parts. */
int func_08A1A978(Part08A1A978Task *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A1AE08(self, x, y);
    func_08A01948(self->part_1C);
    func_08A18F00(self->part_44);
    func_08A01F94(self->part_68);
    func_08A10C88(self->part_8C);
    func_089F83AC(self->part_B8);
    func_089F602C(self->part_F0, x);
    func_08A08334(self->part_138);
    return 0;
}

}
