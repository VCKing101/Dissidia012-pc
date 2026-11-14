#include "battle/agent11_types.h"

class Part089FD7F0Task : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x8];
    char part_14[0x28];
    char part_3C[0x30];
    char part_6C[0x34];
    char part_A0[0x4];
};

extern "C" {

extern void func_089FC3E4(void *part);
extern int func_089FDA44(Part089FD7F0Task *self, float x, float y);
extern void func_089FE054(void *part);
extern void func_08A01948(void *part);
extern void func_08A169DC(void *part);

/* Runs the current state, then updates the task's parts. */
int func_089FD7F0(Part089FD7F0Task *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_089FDA44(self, x, y);
    func_08A01948(self->part_14);
    func_08A169DC(self->part_3C);
    func_089FE054(self->part_6C);
    func_089FC3E4(self->part_A0);
    return 0;
}

}
