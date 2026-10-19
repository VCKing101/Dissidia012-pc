/* Offset-only partial views for observed aligned member stores. */
typedef struct BattleTableAt94 { unsigned char padding[148]; void *member_table; } BattleTableAt94;
typedef struct BattleTableAt8 { unsigned char padding[8]; void *member_table; } BattleTableAt8;
typedef struct BattleTableAt1C0 { unsigned char padding[448]; void *member_table; } BattleTableAt1C0;
typedef struct BattleTableAt11DC { unsigned char padding[4572]; void *member_table; } BattleTableAt11DC;
typedef struct BattleTableAt44 { unsigned char padding[68]; void *member_table; } BattleTableAt44;
typedef struct BattleTableAt864 { unsigned char padding[2148]; void *member_table; } BattleTableAt864;
typedef struct BattleTableAt4 { unsigned char padding[4]; void *member_table; } BattleTableAt4;
typedef struct BattleTableAt20 { unsigned char padding[32]; void *member_table; } BattleTableAt20;
typedef struct BattleResetPrefix { unsigned char field_00; unsigned char padding_01[3]; unsigned int field_04; } BattleResetPrefix;
typedef struct BattleReset32D0 { unsigned char field_00; unsigned char padding_01[3]; unsigned int field_04; unsigned char padding_08[0x1B0]; unsigned char field_1B8[3]; unsigned char field_1BB; unsigned char field_1BC; } BattleReset32D0;
typedef struct BattleReset6998 { unsigned char field_00; unsigned char field_01[4]; unsigned char field_05; } BattleReset6998;
typedef struct BattleReset7AD0 { unsigned char field_00; unsigned char padding_01[0x643]; unsigned char field_644[2]; unsigned char padding_646[2]; unsigned int field_648; unsigned int field_64C; } BattleReset7AD0;
typedef struct BattleResetCFD0 { unsigned char field_00; unsigned char padding_01[3]; unsigned int field_04; int field_08; unsigned int field_0C; int field_10; float field_14; unsigned char field_18[2]; unsigned char field_1A; unsigned char field_1B; unsigned int field_1C; } BattleResetCFD0;

/* Constructor-like helpers reached from the reviewed battle setup root.
 * Member tables are preserved symbols; original class layouts are unknown. */
extern unsigned char D_089E0CA8[], D_089E11B8[], D_089E1060[];
extern unsigned char D_089E38C0[], D_089E38D0[], D_089E6E50[];
extern unsigned char D_089E70D8[], D_089E7390[], D_089E73A0[];
extern unsigned char D_089E8228[], D_089E5218[], D_089E8F68[];
extern unsigned char D_089E8F58[], D_089ED850[];
extern void func_0887A61C(void *);
extern void func_08993560(void *, unsigned int, unsigned int, void *);
extern void func_089E1D00(void *);
extern void func_089E9E10(void *);
extern void func_089DDFC8(void *);
extern void func_089E0D48(void *);
extern void func_089E6EF0(void *);

void func_089E07A0(unsigned char *object);
void func_089E32D0(unsigned char *object);
void func_089E6998(unsigned char *object);
void func_089E71F8(unsigned char *object);
void func_089E7AD0(unsigned char *object);
void func_089E8EDC(unsigned int *object);
void func_089ECFD0(unsigned char *object);
void *func_089E0CB8(unsigned char *object);
void *func_089E6E60(unsigned char *object);

void *func_089E06A8(unsigned char *object)
{
    ((BattleTableAt94 *)object)->member_table = D_089E0CA8;
    func_0887A61C(object + 8);
    func_08993560(object + 0x18, 4, 0x1C, D_089E11B8);
    func_089E0CB8(object + 0x88);
    func_089E07A0(object);
    return object;
}

void *func_089E0CB8(unsigned char *object)
{
    ((BattleTableAt8 *)object)->member_table = D_089E1060;
    func_089E0D48(object);
    return object;
}

void *func_089E3208(unsigned char *object)
{
    ((BattleTableAt1C0 *)object)->member_table = D_089E38C0;
    func_08993560(object + 8, 2, 0xD8, D_089E38D0);
    func_089E32D0(object);
    return object;
}

void *func_089E68E0(unsigned char *object)
{
    ((BattleTableAt11DC *)object)->member_table = D_089E6E50;
    func_089E1D00(object + 8);
    func_089E9E10(object + 0x124);
    func_089E6998(object);
    return object;
}

void *func_089E6E60(unsigned char *object)
{
    ((BattleTableAt8 *)object)->member_table = D_089E70D8;
    func_089E6EF0(object);
    return object;
}

void *func_089E7130(unsigned char *object)
{
    ((BattleTableAt44 *)object)->member_table = D_089E7390;
    func_08993560(object + 4, 2, 0x20, D_089E73A0);
    func_089E71F8(object);
    return object;
}

void *func_089E79A8(unsigned char *object)
{
    ((BattleTableAt864 *)object)->member_table = D_089E8228;
    func_089DDFC8(object + 4);
    func_08993560(object + 0x650, 2, 0xDC, D_089E5218);
    func_08993560(object + 0x808, 2, 0x28, D_089E8F68);
    func_089E6E60(object + 0x858);
    func_089E7AD0(object);
    return object;
}

void *func_089E8E70(unsigned char *object)
{
    ((BattleTableAt4 *)object)->member_table = D_089E8F58;
    func_089E8EDC((unsigned int *)object);
    return object;
}

void *func_089ECF40(unsigned char *object)
{
    ((BattleTableAt20 *)object)->member_table = D_089ED850;
    func_089ECFD0(object);
    return object;
}

void func_089E07A0(unsigned char *object)
{
    ((BattleResetPrefix *)object)->field_00 = 0;
    ((BattleResetPrefix *)object)->field_04 = 0;
}

void func_089E32D0(unsigned char *object)
{
    int i;
    ((BattleReset32D0 *)object)->field_00 = 0;
    ((BattleReset32D0 *)object)->field_04 = 0;
    for (i = 0; i < 3; ++i) *(object + i + 0x1B8) = 0;
    ((BattleReset32D0 *)object)->field_1BB = 0;
    ((BattleReset32D0 *)object)->field_1BC = 0;
}

void func_089E6998(unsigned char *object)
{
    int i;
    ((BattleReset6998 *)object)->field_00 = 0;
    for (i = 0; i < 4; ++i) *(object + i + 1) = 0;
    ((BattleReset6998 *)object)->field_05 = 0;
}

void func_089E71F8(unsigned char *object) { object[0] = 0; }

void func_089E7AD0(unsigned char *object)
{
    int i;
    ((BattleReset7AD0 *)object)->field_00 = 0;
    for (i = 0; i < 2; ++i) *(object + i + 0x644) = 0;
    ((BattleReset7AD0 *)object)->field_648 = 0;
    ((BattleReset7AD0 *)object)->field_64C = 0;
}

void func_089E8EDC(unsigned int *object) { *object = 1; }

void func_089ECFD0(unsigned char *object)
{
    int i;
    ((BattleResetCFD0 *)object)->field_00 = 0;
    ((BattleResetCFD0 *)object)->field_04 = 0;
    ((BattleResetCFD0 *)object)->field_08 = -1;
    ((BattleResetCFD0 *)object)->field_0C = 0;
    ((BattleResetCFD0 *)object)->field_10 = -1;
    ((BattleResetCFD0 *)object)->field_14 = 0.0f;
    for (i = 0; i < 2; ++i) *(object + i + 0x18) = 0;
    ((BattleResetCFD0 *)object)->field_1A = 0;
    ((BattleResetCFD0 *)object)->field_1B = 0;
    ((BattleResetCFD0 *)object)->field_1C = 0;
}
