extern "C" {
#include "battle/runtime_helpers.h"
}

/* Private minimum-offset views; no original class identities are asserted. */
struct BattleCurveView {
    unsigned char field_00;
    unsigned char padding_01[3];
    unsigned int field_04;
    void *field_08;
    int field_0C;
    int field_10;
    float field_14;
    float field_18;
};
struct BattleListLinkView {
    unsigned char padding_00[0x10];
    unsigned int field_10;
    unsigned char padding_14[0x38];
    BattleListLinkView *field_4C;
    unsigned char padding_50[4];
    BattleListLinkView *field_54;
};
struct BattleListOwnerView {
    unsigned char padding_00[0x30];
    BattleListLinkView *field_30;
    BattleListLinkView *field_34;
    unsigned int field_38;
};
struct BattleCleanupNodeView {
    unsigned char padding_00[0x10];
    BattleListOwnerView *field_10;
    BattleListLinkView *field_14[4];
};
struct BattleListOps {
    static BattleListOwnerView *owner(BattleCleanupNodeView *state) { return state->field_10; }
    static void detach(BattleListOwnerView *owner, BattleListLinkView *link) {
        if (owner && link) {
            link->field_10 &= ~0x8000u;
            if (!link->field_54) owner->field_30 = link->field_4C;
            else link->field_54->field_4C = link->field_4C;
            if (!link->field_4C) owner->field_34 = link->field_54;
            else link->field_4C->field_54 = link->field_54;
            link->field_54 = 0;
            link->field_4C = 0;
            --owner->field_38;
        }
    }
};

extern "C" {
extern float D_089D5EAC, D_089E68B0, D_089E68B4;
extern float func_089E6444(void *);
extern int func_089E973C(unsigned int);
extern void func_0887BA5C(void *);
extern void func_088DA060(void *);
extern void func_089EBC54(void *);

void func_089E6450(void *object, int current)
{
    BattleCurveView *state = (BattleCurveView *)object;
    switch (state->field_0C) {
    case 0:
        if (current < state->field_10) {
            float previous = (float)state->field_10;
            state->field_0C = 1;
            state->field_18 = 0.0f;
            state->field_14 = previous;
        }
        break;
    case 1:
        {
            float value = (float)current;
            if (!(value < state->field_14)) state->field_0C = 0;
            if (current < state->field_10) state->field_18 = 0.0f;
            else {
                float delta = D_089D5EAC;
                float progress = state->field_18 + delta;
                state->field_18 = progress;
                if (!(progress <= func_089E6444(state))) {
                    float origin = state->field_14;
                    int limit = func_089E973C(state->field_04);
                    state->field_14 = origin - delta * (float)limit / D_089E68B4;
                    if (!(value < state->field_14)) state->field_0C = 0;
                }
            }
        }
    }
    state->field_10 = current;
}

float func_089E6444(void *unused_object) { return D_089E68B0; }

void func_089EBCC8(BattleCleanupNodeView *state)
{
    for (int i = 0; i < 4; ++i) {
        BattleListLinkView *node = state->field_14[i];
        if (node) {
            BattleListOps::detach(BattleListOps::owner(state), node);
            if (state->field_14[i]) func_0887BA5C(state->field_14[i]);
            state->field_14[i] = 0;
        }
    }
    if (state->field_10) {
        func_088DA060(state->field_10);
        state->field_10 = 0;
    }
    func_089EBC54(state);
}
}
