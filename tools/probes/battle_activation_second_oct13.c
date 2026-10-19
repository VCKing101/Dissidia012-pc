#include "battle/runtime_helpers.h"

typedef struct BattleOct13Peer198 {
    unsigned char padding_00[0x16C];
    void *field_16C;
    unsigned char padding_170[0x28];
    void *field_198;
} BattleOct13Peer198;
typedef struct BattleOct13Peer1CC { unsigned char padding[0x1CC]; void *field_1CC; } BattleOct13Peer1CC;
typedef struct BattleOct13Prefix04 { unsigned char field_00; unsigned char padding_01[3]; void *field_04; } BattleOct13Prefix04;
typedef struct BattleOct13Bind14 {
    unsigned char field_00;
    unsigned char padding_01[0xB];
    unsigned int field_0C;
    void *field_10;
    void *field_14;
} BattleOct13Bind14;
typedef struct BattleOct13Bind18 {
    unsigned char field_00;
    unsigned char padding_01[0xF];
    unsigned int field_10;
    void *field_14;
    void *field_18;
} BattleOct13Bind18;
typedef struct BattleOct13FourResources {
    unsigned char field_00;
    unsigned char padding_01[3];
    float field_04;
    void *field_08;
    void *field_0C;
    void *field_10;
    void *field_14;
    unsigned int field_18;
    void *field_1C;
    void *field_20;
} BattleOct13FourResources;

extern void *D_089D7830;
extern unsigned char D_089D5F1F;
extern int D_089E9578[2], D_089E9580[2];
extern void *func_088D09D8(void *, int, int);
extern void *func_088DAA40(void *, int, int);
extern void func_088DA060(void *);
extern void func_088D9F00(void *, float);
extern int func_089E95E0(unsigned int);
extern float func_089E97FC(unsigned int);
extern int func_089E9C64(void);
extern void func_089E0C6C(void *);
extern void *func_088908E0(void);
extern int func_0887AC70(void *, void *, int, void *);
extern void func_089E3300(void *);
extern int func_089E3A0C(void *, unsigned int, void *, void *);
extern void func_089E7200(void *);
extern void func_089E8FF8(void *);

int func_089E12A8(void *, unsigned int, void *, void *);
int func_089E0D70(void *, void *);
void func_089E0D54(void *);
int func_089E74A8(void *, unsigned int, void *, void *);
void func_089E9024(void *);

int func_089E082C(void *object)
{
    func_089E07AC((unsigned char *)object);
    ((BattleOct13Prefix04 *)object)->field_04 = func_088D09D8(D_089D7830, 1, 0x15);
    if (!((BattleOct13Prefix04 *)object)->field_04) return -1;
    {
        int i = 0;
        unsigned char *shared = (unsigned char *)object + 8;
        unsigned char *entry = (unsigned char *)object + 0x18;
        do {
            if (func_089E12A8(entry, i, ((BattleOct13Prefix04 *)object)->field_04, shared)) return -2;
            ++i;
            entry += 0x1C;
        } while (i < 4);
    }
    if (func_089E0D70((unsigned char *)object + 0x88, ((BattleOct13Prefix04 *)object)->field_04)) return -3;
    ((BattleOct13Prefix04 *)object)->field_00 = 1;
    return 0;
}

int func_089E3358(void *object, BattleOct13Peer198 *peer)
{
    func_089E3300(object);
    if (!peer || !peer->field_198 || !peer->field_16C) return -1;
    {
        int i = 0;
        unsigned char *entry = (unsigned char *)object + 8;
        do {
            if (func_089E3A0C(entry, i, peer->field_198, peer->field_16C)) return -2;
            ++i;
            entry += 0xD8;
        } while (i < 2);
    }
    ((BattleOct13Prefix04 *)object)->field_04 = peer;
    ((BattleOct13Prefix04 *)object)->field_00 = 1;
    return 0;
}

int func_089E7258(void *object, BattleOct13Peer1CC *peer, void *shared)
{
    func_089E7200(object);
    if (!peer || !peer->field_1CC || !shared) return -1;
    {
        int i = 0;
        unsigned char *entry = (unsigned char *)object + 4;
        do {
            if (func_089E74A8(entry, i, peer->field_1CC, shared)) return -2;
            ++i;
            entry += 0x20;
        } while (i < 2);
    }
    ((BattleOct13Prefix04 *)object)->field_00 = 1;
    return 0;
}

int func_089ED064(void *object)
{
    func_089ED01C((unsigned char *)object);
    ((BattleOct13Prefix04 *)object)->field_04 = func_088D09D8(D_089D7830, 1, 0x1F);
    if (!((BattleOct13Prefix04 *)object)->field_04) return -1;
    ((BattleOct13Prefix04 *)object)->field_00 = 1;
    return 0;
}

int func_089E12A8(void *object, unsigned int index, void *resource, void *shared)
{
    func_089E1268(object);
    if (index >= 4 || !resource || !shared) return -1;
    ((BattleOct13Bind14 *)object)->field_0C = index;
    ((BattleOct13Bind14 *)object)->field_10 = resource;
    ((BattleOct13Bind14 *)object)->field_14 = shared;
    ((BattleOct13Bind14 *)object)->field_00 = 1;
    return 0;
}

int func_089E0D70(void *object, void *resource)
{
    func_089E0D54(object);
    if (!resource) return -1;
    ((BattleOct13Prefix04 *)object)->field_04 = resource;
    ((BattleOct13Prefix04 *)object)->field_00 = 1;
    return 0;
}

int func_089E74A8(void *object, unsigned int index, void *resource, void *shared)
{
    func_089E7450((BattleRelease7450 *)object);
    if (index >= 2 || !resource || !shared) return -1;
    ((BattleOct13Bind18 *)object)->field_10 = index;
    ((BattleOct13Bind18 *)object)->field_14 = resource;
    ((BattleOct13Bind18 *)object)->field_18 = shared;
    ((BattleOct13Bind18 *)object)->field_00 = 1;
    return 0;
}

void func_089E0D54(void *object) { func_089E0D48((unsigned char *)object); }

int func_089E0BE8(void *object, void *buffer, int count)
{
    func_089E0C6C(object);
    if (!buffer || count <= 0) return -1;
    if (!func_0887AC70((unsigned char *)object + 8, buffer, count, func_088908E0())) return -2;
    return 0;
}

int func_089E8EF0(void *object)
{
    int value = 0;
    if (!D_089D5F1F) {
        int mode = func_089E9C64();
        value = *(int *)object;
        if (mode == 1) {
            if (value == 1) value = 0;
        }
    }
    return value;
}

void func_089E9024(void *object)
{
    BattleOct13FourResources *state = (BattleOct13FourResources *)object;
    if (state->field_14) { func_088DA060(state->field_14); state->field_14 = 0; }
    if (state->field_10) { func_088DA060(state->field_10); state->field_10 = 0; }
    if (state->field_0C) { func_088DA060(state->field_0C); state->field_0C = 0; }
    if (state->field_08) { func_088DA060(state->field_08); state->field_08 = 0; }
    func_089E8FF8(state);
}

int func_089E90AC(void *object, unsigned int index, void *owner, void *shared)
{
    func_089E9024(object);
    if (index < 2 && owner && shared) {
        int mode = func_089E95E0(index);
        if ((unsigned int)mode >= 2) return -2;
        ((BattleOct13FourResources *)object)->field_10 = func_088DAA40(owner, D_089E9578[mode], 0);
        if (!((BattleOct13FourResources *)object)->field_10) return -3;
        ((BattleOct13FourResources *)object)->field_14 = func_088DAA40(owner, D_089E9580[mode], 0);
        if (!((BattleOct13FourResources *)object)->field_14) return -4;
        func_088D9F00(((BattleOct13FourResources *)object)->field_14, 0.0f);
        ((BattleOct13FourResources *)object)->field_04 = func_089E97FC(index);
        ((BattleOct13FourResources *)object)->field_18 = index;
        ((BattleOct13FourResources *)object)->field_1C = owner;
        ((BattleOct13FourResources *)object)->field_20 = shared;
        ((BattleOct13FourResources *)object)->field_00 = 1;
        return 0;
    }
    return -1;
}

extern void func_089E32D0(unsigned char *);
extern void func_089E39CC(void *);
extern int func_0887AFB0(void *);
extern void func_0887AD1C(void *);
void func_089E3300(void *object)
{
    int i = 0;
    unsigned char *entry = (unsigned char *)object + 8;
    do {
        func_089E39CC(entry);
        ++i;
        entry += 0xD8;
    } while (i < 2);
    func_089E32D0((unsigned char *)object);
}
void func_089E7200(void *object)
{
    int i = 0;
    unsigned char *entry = (unsigned char *)object + 4;
    do {
        func_089E7450((BattleRelease7450 *)entry);
        ++i;
        entry += 0x20;
    } while (i < 2);
    func_089E71F8((unsigned char *)object);
}
void func_089E0C6C(void *object)
{
    void *block = (unsigned char *)object + 8;
    if (func_0887AFB0(block) == 1) func_0887AD1C(block);
}
void func_089E8FF8(void *object)
{
    BattleOct13FourResources *state = (BattleOct13FourResources *)object;
    state->field_00 = 0;
    state->field_04 = 0.0f;
    state->field_08 = 0;
    state->field_0C = 0;
    state->field_10 = 0;
    state->field_14 = 0;
    state->field_18 = 0;
    state->field_1C = 0;
    state->field_20 = 0;
}
