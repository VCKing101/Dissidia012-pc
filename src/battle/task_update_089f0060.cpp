#include "battle/agent11_types.h"

class Part089F0060Task : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x10];
    char part_1C[0x28];
    char part_44[0x38];
    char part_7C[0x98];
    char part_114[0x4];
};

extern "C" {

extern void func_089EF87C(void *part, float x, float y);
extern int func_089F0500(Part089F0060Task *self, float x, float y);
extern void func_089F83AC(void *part);
extern void func_08A08334(void *part);
extern void func_08A0E898(void *part);

/* Runs the current state, then updates the task's parts. */
int func_089F0060(Part089F0060Task *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_089F0500(self, x, y);
    func_089EF87C(self->part_1C, x, y);
    func_089F83AC(self->part_44);
    func_08A0E898(self->part_7C);
    func_08A08334(self->part_114);
    return 0;
}

}
