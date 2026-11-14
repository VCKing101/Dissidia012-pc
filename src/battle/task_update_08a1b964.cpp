#include "battle/agent11_types.h"

class Part08A1B964Task : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0xC];
    char part_18[0x28];
    char part_40[0x24];
    char part_64[0x24];
    char part_88[0x78];
    char part_100[0x38];
    char part_138[0x4];
};

extern "C" {

extern void func_089F83AC(void *part);
extern void func_08A01948(void *part);
extern void func_08A01F94(void *part);
extern void func_08A08334(void *part);
extern void func_08A18F00(void *part);
extern int func_08A1BDE4(Part08A1B964Task *self, float x, float y);
extern void func_08A21740(void *part);

/* Runs the current state, then updates the task's parts. */
int func_08A1B964(Part08A1B964Task *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A1BDE4(self, x, y);
    func_08A01948(self->part_18);
    func_08A18F00(self->part_40);
    func_08A01F94(self->part_64);
    func_08A21740(self->part_88);
    func_089F83AC(self->part_100);
    func_08A08334(self->part_138);
    return 0;
}

}
