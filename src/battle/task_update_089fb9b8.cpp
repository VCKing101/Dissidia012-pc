#include "battle/agent11_types.h"

class Part089FB9B8Task : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0xC];
    char part_18[0x78];
    char part_90[0x38];
    char part_C8[0x4];
};

extern "C" {

extern void func_089F83AC(void *part);
extern int func_089FBD10(Part089FB9B8Task *self, float x, float y);
extern void func_08A08334(void *part);
extern void func_08A21740(void *part);

/* Runs the current state, then updates the task's parts. */
int func_089FB9B8(Part089FB9B8Task *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_089FBD10(self, x, y);
    func_08A21740(self->part_18);
    func_089F83AC(self->part_90);
    func_08A08334(self->part_C8);
    return 0;
}

}
