#include "battle/runtime_helpers.h"

typedef struct BattleActivationHeader {
    unsigned char field_00;
    unsigned char padding_01[3];
    void *field_04;
    void *field_08;
    unsigned char padding_0C[0xC];
    void *field_18;
} BattleActivationHeader;
typedef struct BattleInput1C4 { unsigned char padding[0x1C4]; void *field_1C4; } BattleInput1C4;
typedef struct BattleBinding2B48 {
    unsigned char field_00;
    unsigned char padding_01[0xF];
    unsigned int field_10;
    void *field_14;
} BattleBinding2B48;
typedef struct BattleSlots114 {
    unsigned char field_00;
    unsigned char padding_01[0x113];
    unsigned char field_114[3];
} BattleSlots114;
typedef struct BattleGlobal278 { unsigned char padding[0x278]; void *field_278; } BattleGlobal278;
typedef struct BattleResourceFlags { unsigned char padding[0x20]; unsigned short field_20; } BattleResourceFlags;
typedef struct BattleState56EC {
    unsigned char padding_00[4];
    void *field_04;
    unsigned char padding_08[4];
    int field_0C;
    unsigned char padding_10[0xC4];
    BattleResourceFlags *field_D4;
} BattleState56EC;
extern void *D_089D7830, *D_089D6998;
extern BattleGlobal278 *D_08A405E0;
extern void func_089E1E30(void *);
extern void *func_088D09D8(void *, int, int);
extern void *func_0887E0CC(void *, int);
extern int func_089E2304(void *, unsigned int, void *);
extern void *func_088DAA40(void *, int, int);
extern int func_089EA7B0(void *, void *, unsigned int, void *, void *);
extern void func_089EA74C(void *);
extern int func_089E21D0(void *);
extern int func_08A50140(void);
extern int func_089E96DC(void *);
extern void func_088D9CD8(void *);
extern void func_088D9AEC(void *, float);
extern void func_088D9CFC(void *);
extern void func_088D9D24(void *);
extern void func_089E63D4(void *);
extern void func_089E5B60(void *);
extern void func_089E4EE0(void *);
extern void func_089EE014(void *);
extern void func_089E439C(void *);
extern void func_089E2B2C(void *);
extern int func_0887AFB0(void *);
extern void func_088DA060(void *);

int func_089E2B48(unsigned char *, unsigned int, void *);
int func_089E20FC(unsigned char *, unsigned int);
int func_089E2164(unsigned char *, unsigned int);
int func_089EA0AC(unsigned char *, int);
void func_089E9F04(unsigned char *);
void func_089E9EF4(unsigned char *);
unsigned char func_089EA11C(unsigned char *);

int func_089E1EC0(unsigned char *object, void *input)
{
    func_089E1E30(object);
    if (!input) return -1;
    ((BattleActivationHeader *)object)->field_08 = func_088D09D8(D_089D7830, 1, 0xD);
    if (!((BattleActivationHeader *)object)->field_08) return -2;
    {
        int i;
        unsigned char *entry_a, *entry_b;
        i = 0;
        entry_a = object + 0xC;
        entry_b = object + 0x44;
        do {
            if (func_089E2B48(entry_a, i, *(void **)((BattleActivationHeader *)object)->field_08)) return -4;
            if (func_089E2304(entry_b, i, func_0887E0CC(D_089D6998, 0))) return -5;
            ++i;
            entry_a += 0x1C;
            entry_b += 0x68;
        } while (i < 2);
    }
    ((BattleActivationHeader *)object)->field_04 = input;
    object[0] = 1;
    return 0;
}

int func_089E9F74(unsigned char *object, BattleInput1C4 *input_a, void *input_b)
{
    func_089E9F04(object);
    if (!input_a || !input_a->field_1C4 || !input_b) return -1;
    ((BattleActivationHeader *)object)->field_18 = func_088DAA40(input_a->field_1C4, 0xD, 0);
    if (!((BattleActivationHeader *)object)->field_18) return -2;
    {
        int i;
        unsigned char *shared, *entry;
        i = 0;
        entry = object + 0x1C;
        shared = object + 8;
        do {
            if (func_089EA7B0(entry, input_b, i, shared, input_a->field_1C4)) return -3;
            ++i;
            entry += 0x84C;
        } while (i < 2);
    }
    ((BattleActivationHeader *)object)->field_04 = input_b;
    object[0] = 1;
    return 0;
}

int func_089E2B48(unsigned char *object, unsigned int index, void *input)
{
    func_089E2B2C(object);
    if (index >= 2 || !input) return -1;
    ((BattleBinding2B48 *)object)->field_10 = index;
    ((BattleBinding2B48 *)object)->field_14 = input;
    object[0] = 1;
    return 0;
}

int func_089E20FC(unsigned char *object, unsigned int index)
{
    if (!object[0]) return -1;
    if (index >= 3) return -2;
    ((BattleSlots114 *)object)->field_114[index] = 0;
    if (func_089E21D0(object)) return -3;
    return 0;
}

int func_089E2164(unsigned char *object, unsigned int index)
{
    if (!object[0]) return -1;
    if (index >= 3) return -2;
    ((BattleSlots114 *)object)->field_114[index] = 1;
    if (func_089E21D0(object)) return -3;
    return 0;
}

int func_089E6CE4(unsigned char *object)
{
    unsigned int global_flag = (unsigned char)(D_08A405E0->field_278 != 0);
    unsigned char *entry_a = object + 8;
    unsigned char *entry_b = object + 0x124;
    if (global_flag == 1) {
        func_089E2164(entry_a, 1);
        func_089EA0AC(entry_b, 1);
    } else if (func_08A50140() == 1) {
        func_089E2164(entry_a, 1);
        func_089EA0AC(entry_b, 0);
    } else {
        func_089E20FC(entry_a, 1);
        func_089EA0AC(entry_b, 0);
    }
    return 0;
}

int func_089EA0AC(unsigned char *object, int value)
{
    if (!func_089EA11C(object)) return -1;
    {
        int i;
        unsigned char *entry;
        i = 0;
        entry = object + 0x1C;
        do {
            extern void func_089EA8F8(void *, int);
            func_089EA8F8(entry, value);
            ++i;
            entry += 0x84C;
        } while (i < 2);
    }
    return 0;
}

unsigned char func_089EA11C(unsigned char *object)
{
    unsigned int result = 0;
    if (object[0] == 1 && func_0887AFB0(object + 8) == 1) result = 1;
    return (unsigned char)result;
}

int func_089E56EC(BattleState56EC *state)
{
    int current = func_089E96DC(state->field_04);
    if (current < state->field_0C) {
        if ((unsigned char)!!(state->field_D4->field_20 & 8) == 0) func_088D9CD8(state->field_D4);
        func_088D9AEC(state->field_D4, 0.0f);
    }
    if ((unsigned char)!!(state->field_D4->field_20 & 8) == 1) {
        if ((unsigned char)!!(state->field_D4->field_20 & 0x20) == 1) {
            func_088D9CFC(state->field_D4);
            state->field_D4->field_20 &= ~0x20;
            func_088D9D24(state->field_D4);
        }
    }
    state->field_0C = current;
    return 0;
}

int func_089E5988(unsigned char *object)
{
    func_089E63D4(object + 0x10);
    func_089E5B60(object + 0x40);
    func_089E4EE0(object + 0x58);
    func_089EE014(object + 0x84);
    return 0;
}

int func_089E4368(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089E439C(object);
    return 0;
}

void func_089E9F04(unsigned char *object)
{
    int i;
    unsigned char *entry;
    void *resource;
    i = 0;
    entry = object + 0x1C;
    do {
        func_089EA74C(entry);
        ++i;
        entry += 0x84C;
    } while (i < 2);
    resource = ((BattleActivationHeader *)object)->field_18;
    if (resource) {
        func_088DA060(resource);
        ((BattleActivationHeader *)object)->field_18 = 0;
    }
    func_089E9EF4(object);
}

void func_089E9EF4(unsigned char *object)
{
    object[0] = 0;
    ((BattleActivationHeader *)object)->field_04 = 0;
    ((BattleActivationHeader *)object)->field_18 = 0;
}
