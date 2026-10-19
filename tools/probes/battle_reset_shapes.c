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

void func_089E32D0(BattleReset32D0 *object)
{
    int i;
    object->field_00 = 0;
    object->field_04 = 0;
    for (i = 0; i < 3; ++i) object->field_1B8[i] = 0;
    object->field_1BB = 0;
    object->field_1BC = 0;
}

void func_089E6998(BattleReset6998 *object)
{
    int i;
    object->field_00 = 0;
    for (i = 0; i < 4; ++i) object->field_01[i] = 0;
    object->field_05 = 0;
}

void func_089E7AD0(BattleReset7AD0 *object)
{
    int i;
    object->field_00 = 0;
    for (i = 0; i < 2; ++i) object->field_644[i] = 0;
    object->field_648 = 0;
    object->field_64C = 0;
}

void func_089ECFD0(BattleResetCFD0 *object)
{
    int i;
    object->field_00 = 0;
    object->field_04 = 0;
    object->field_08 = -1;
    object->field_0C = 0;
    object->field_10 = -1;
    object->field_14 = 0.0f;
    for (i = 0; i < 2; ++i) object->field_18[i] = 0;
    object->field_1A = 0;
    object->field_1B = 0;
    object->field_1C = 0;
}

