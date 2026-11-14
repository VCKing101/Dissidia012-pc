#include "battle/agent11_types.h"

class Part089F6948Task : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x10];
    char part_1C[0x2C];
    char part_48[0x34];
    char part_7C[0x4];
};

extern "C" {

extern int func_089F6D6C(Part089F6948Task *self, float x, float y);
extern void func_089F83AC(void *part);
extern void func_08A08334(void *part);
extern void func_08A10C88(void *part);

/* Runs the current state, then updates the task's parts. */
int func_089F6948(Part089F6948Task *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_089F6D6C(self, x, y);
    func_08A10C88(self->part_1C);
    func_08A08334(self->part_48);
    func_089F83AC(self->part_7C);
    return 0;
}

}
