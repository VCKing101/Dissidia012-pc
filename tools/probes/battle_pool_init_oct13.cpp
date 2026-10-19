extern "C" {
#include "battle/runtime_helpers.h"
}

struct BattlePoolSlotView { void *field_00; BattlePoolSlotView *field_04; };
struct BattlePoolInitView {
    unsigned char field_00;
    unsigned char padding_01[3];
    int field_04;
    BattlePoolSlotView field_08[16];
    unsigned char field_88[0x780];
    void *field_808;
    void *field_80C;
    void *field_810;
    unsigned int field_814;
    unsigned int field_818;
    unsigned char padding_81C[0x10];
    unsigned char field_82C[2];
    unsigned char field_82E;
    unsigned char padding_82F;
    unsigned int field_830;
    float field_834;
    void *field_838;
    int field_83C;
    void *field_840;
    void *field_844;
};
extern "C" void *func_08935828(void *, int, unsigned int);

extern "C" void func_089EA680(BattlePoolInitView *state)
{
    state->field_00 = 0;
    state->field_04 = -1;
    {
        int i = 0;
        unsigned char *data = state->field_88;
        do {
            state->field_08[i].field_00 = data;
            state->field_08[i].field_04 = &state->field_08[i + 1];
            ++i;
            data += 0x78;
        } while (i < 16);
    }
    func_08935828(state->field_88, 0, 0x780);
    state->field_08[15].field_04 = state->field_08;
    state->field_808 = state->field_08;
    state->field_80C = state->field_08;
    state->field_814 = 0;
    state->field_810 = 0;
    state->field_818 = 0;
    for (int i = 0; i < 2; ++i) state->field_82C[i] = 0;
    state->field_82E = 0;
    state->field_830 = 0;
    state->field_834 = 0.0f;
    state->field_838 = 0;
    state->field_83C = -1;
    state->field_840 = 0;
    state->field_844 = 0;
}
