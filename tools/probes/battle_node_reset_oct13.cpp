extern "C" {
#include "battle/runtime_helpers.h"
void func_088DA060(void *);
}
struct BattleNodeResetView {
    unsigned char padding_00[8];
    unsigned char field_08;
    unsigned char padding_09[3];
    int field_0C;
    void *field_10;
    void *field_14[4];
    unsigned char field_24;
    unsigned char padding_25[3];
    float field_28;
    unsigned char field_2C;
    unsigned char field_2D;
    unsigned char field_2E;
    unsigned char padding_2F;
    int field_30;
    float field_34;
    float field_38;
    void *field_3C;
    void *field_40;
};
struct BattleChildResourceView {
    unsigned char field_00;
    unsigned char padding_01[3];
    int field_04;
    void *field_08;
};
extern "C" void func_089EB804(void *);
extern "C" void func_089EB8F0(void *);

extern "C" void func_089EBC54(void *object)
{
    BattleNodeResetView *state = (BattleNodeResetView *)object;
    state->field_08 = 0;
    state->field_0C = -1;
    state->field_10 = 0;
    for (int i = 0; i < 4; ++i) state->field_14[i] = 0;
    state->field_24 = 0;
    state->field_28 = 0.0f;
    state->field_2C = 0;
    state->field_2D = 0;
    state->field_2E = 0;
    state->field_30 = -1;
    state->field_34 = 0.0f;
    state->field_38 = 0.0f;
    state->field_3C = 0;
    state->field_40 = 0;
    func_089EB804((unsigned char *)object + 0x44);
}

extern "C" void func_089EB904(void *object)
{
    BattleChildResourceView *state = (BattleChildResourceView *)object;
    if (state->field_08) {
        func_088DA060(state->field_08);
        state->field_08 = 0;
    }
    func_089EB8F0(object);
}
