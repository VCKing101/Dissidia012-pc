#include "battle/runtime_helpers.h"

typedef struct BattleCounterBounds { unsigned char padding[8]; int field_08; int field_0C; } BattleCounterBounds;
typedef struct BattleLookupContext { unsigned char padding[0x773]; signed char field_773; } BattleLookupContext;
typedef struct BattleLookupState {
    unsigned char padding_00[0x30C];
    BattleLookupContext *field_30C;
    unsigned char padding_310[0x21C];
    BattleCounterBounds *field_52C;
} BattleLookupState;
typedef struct BattleLookupController { unsigned char padding[0x38]; BattleLookupState *field_38[2]; } BattleLookupController;
typedef struct BattleValue13 { unsigned char padding[0x13]; signed char field_13; } BattleValue13;
typedef struct BattleValueSlot { int field_04; float field_08; } BattleValueSlot;
typedef struct BattleLookupBinding {
    unsigned char field_00;
    unsigned char padding_01[3];
    BattleValueSlot field_04[10];
    unsigned char padding_54[8];
    unsigned int field_5C;
    void *field_60;
} BattleLookupBinding;
typedef struct BattleBinding838 {
    unsigned char field_00;
    unsigned char padding_01[3];
    unsigned int field_04;
    unsigned char padding_08[0x830];
    void *field_838;
    unsigned int field_83C;
    void *field_840;
    void *field_844;
} BattleBinding838;
typedef struct BattleBindingB944 { unsigned char field_00; unsigned char padding_01[3]; unsigned int field_04; void *field_08; } BattleBindingB944;
typedef struct BattleRelease7450 { unsigned char padding_00[8]; void *field_08; void *field_0C; } BattleRelease7450;
typedef struct BattleRelease439C { unsigned char padding_00[0xC]; void *field_0C; void *field_10[4][2]; } BattleRelease439C;
typedef struct BattleScale18 { unsigned char padding[0x18]; float field_18; } BattleScale18;
typedef struct BattleSlotIndexPrefix { unsigned char field_00; unsigned char padding_01[3]; unsigned int field_04; } BattleSlotIndexPrefix;
extern BattleLookupController *D_089D6A10;
extern BattleScale18 *D_089D69E0;
extern float D_089E6078;
extern void func_089E22E8(void *);
extern void func_089EA74C(void *);
extern void func_089EB904(void *);
extern BattleValue13 *func_088B29D8(int);
extern void *func_088DAA40(void *, int, int);
extern void func_088DA060(void *);
extern void func_089E7430(void *);
extern void func_089E4A94(void *);
extern void func_089E6450(void *, int);
extern int func_089E6594(void *, int);
extern unsigned char func_089E968C(unsigned int);

BattleLookupState *func_089E95B0(unsigned int index);
int func_089E96DC(unsigned int index);
int func_089EB944(unsigned char *, unsigned int, void *);
int func_089E4A3C(unsigned char *);

BattleLookupState *func_089E95B0(unsigned int index)
{
    if (index >= 2) return 0;
    return D_089D6A10->field_38[index];
}

int func_089E96DC(unsigned int index)
{
    if (index >= 2) return 0;
    {
        BattleLookupState *state = func_089E95B0(index);
        int result = 0;
        if (state) {
            result = (int)((unsigned int)state->field_52C->field_0C - (unsigned int)state->field_52C->field_08);
            result = result >= 0 ? result : 0;
        }
        return result;
    }
}

int func_089E973C(unsigned int index)
{
    if (index >= 2) return 0;
    {
        BattleLookupState *state = func_089E95B0(index);
        int result = 0;
        if (state) result = state->field_52C->field_0C;
        return result;
    }
}

int func_089E95E0(unsigned int index)
{
    if (index >= 2) return -1;
    if (func_089E968C(index) == 1) return 0;
    return -1;
}

int func_089E2304(unsigned char *object, unsigned int index, void *input)
{
    func_089E22E8(object);
    if (index >= 2) return -1;
    if (!input) return -1;
    {
        BattleLookupState *owner = func_089E95B0(index);
        BattleLookupContext *context;
        if (!owner) return -2;
        context = owner->field_30C;
        if (context) {
            int i = 0;
            unsigned char *region = (unsigned char *)context + 0x10;
            signed short *entries = (signed short *)(region + 0xA2 + *(signed char *)(region + 0x763) * 198);
            unsigned char *slot = object;
            for (; i < 10; ++i, ++entries, slot += 8) {
                BattleValue13 *value = func_088B29D8(*entries);
                if (!value) continue;
                ((BattleLookupBinding *)slot)->field_04[0].field_04 = value->field_13;
            }
        }
    }
    ((BattleLookupBinding *)object)->field_5C = index;
    ((BattleLookupBinding *)object)->field_60 = input;
    object[0] = 1;
    return 0;
}

int func_089EA7B0(unsigned char *object, void *input, unsigned int index, void *shared, void *dependency)
{
    func_089EA74C(object);
    if (!input || index >= 2 || !shared || !dependency) return -1;
    if (func_089EB944(object + 0x81C, index, dependency)) return -2;
    ((BattleBinding838 *)object)->field_838 = input;
    ((BattleBinding838 *)object)->field_83C = index;
    ((BattleBinding838 *)object)->field_840 = shared;
    ((BattleBinding838 *)object)->field_844 = dependency;
    ((BattleBinding838 *)object)->field_04 = 0;
    object[0] = 1;
    return 0;
}

int func_089EB944(unsigned char *object, unsigned int index, void *input)
{
    func_089EB904(object);
    if (index >= 2 || !input) return -1;
    ((BattleBindingB944 *)object)->field_08 = func_088DAA40(input, -1, 0);
    if (!((BattleBindingB944 *)object)->field_08) return -2;
    ((BattleBindingB944 *)object)->field_04 = index;
    object[0] = 1;
    return 0;
}

int func_089E5B60(BattleControlState *state)
{
    if (!state->field_00) return -1;
    if (state->field_04 == 2) {
        state->field_08 += D_089E6078 * D_089D69E0->field_18 * 0.5f;
        if (!(state->field_08 <= 1.0f)) state->field_08 = 1.0f;
    }
    return 0;
}

int func_089E63D4(unsigned char *object)
{
    int current;
    if (!object[0]) return -1;
    current = func_089E96DC(((BattleSlotIndexPrefix *)object)->field_04);
    func_089E6450(object, current);
    if (func_089E6594(object, current)) return -2;
    return 0;
}

int func_089E3BF0(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089E4A3C(object + 8);
    func_089E4A3C(object + 0x3C);
    func_089E4368(object + 0x70);
    return 0;
}

int func_089E4A3C(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089E4A94(object);
    return 0;
}

void func_089E7450(BattleRelease7450 *state)
{
    if (state->field_0C) {
        func_088DA060(state->field_0C);
        state->field_0C = 0;
    }
    if (state->field_08) {
        func_088DA060(state->field_08);
        state->field_08 = 0;
    }
    func_089E7430(state);
}

int func_089E439C(unsigned char *object)
{
    int i, j;
    unsigned char *row;
    i = 0;
    row = object;
    do {
        unsigned char *entry;
        j = 0;
        entry = row;
        do {
            void **slot = &((BattleRelease439C *)entry)->field_10[0][0];
            if (*slot) {
                func_088DA060(*slot);
                *slot = 0;
            }
            ++j;
            entry += 4;
        } while (j < 2);
        ++i;
        row += 8;
    } while (i < 4);
    if (((BattleRelease439C *)object)->field_0C) {
        func_088DA060(((BattleRelease439C *)object)->field_0C);
        ((BattleRelease439C *)object)->field_0C = 0;
    }
    return 0;
}
