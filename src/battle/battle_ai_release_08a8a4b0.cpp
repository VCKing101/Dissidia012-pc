#include "battle/agent14_types.h"

class BattleAi : public HandleTaskBase {
public:
    char pad_004[0x38];
    char member_03C[0x10];
    char member_04C[0x28];
    char member_074[0x14];
    char member_088[0x94];
    char member_11C[0x48];
    char member_164[0x1C];
    char member_180[0x10];
    char member_190[0x30];
    char member_1C0[0x4];
};

extern "C" {

extern void func_08A85D14(void *member);
extern void func_08A94300(void *member);
extern void func_08A82220(void *member);
extern void func_08A94E8C(void *member);
extern void func_08AA5914(void *member);
extern void func_08A981C0(void *member);
extern void func_08A8167C(void *member);
extern void func_08AA4328(void *member);
extern void func_08A7E7E0(void *member);

void func_08A8A4B0(BattleAi *self)
{
    func_08A85D14(self->member_1C0);
    func_08A94300(self->member_190);
    func_08A82220(self->member_180);
    func_08A94E8C(self->member_164);
    func_08AA5914(self->member_11C);
    func_08A981C0(self->member_088);
    func_08A8167C(self->member_074);
    func_08AA4328(self->member_04C);
    func_08A7E7E0(self->member_03C);
    self->release();
}

}
