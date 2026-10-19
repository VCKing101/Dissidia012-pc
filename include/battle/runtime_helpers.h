#ifndef DISSIDIA_BATTLE_RUNTIME_HELPERS_H
#define DISSIDIA_BATTLE_RUNTIME_HELPERS_H

/* Only the fields accessed by the matching state-control routines are modeled.
 * The complete owning object and gameplay meaning remain unresolved. */
typedef struct BattleControlState {
    unsigned char field_00;
    unsigned char field_01;
    unsigned char field_02;
    unsigned char padding_03;
    int field_04;
    float field_08;
    int field_0C;
    void *field_10;
} BattleControlState;

void *func_089DD4D0(unsigned char *object);
int func_089DD7A8(unsigned char *object);
int func_089DD854(unsigned char *object);
int func_089DDAF0(unsigned char *object);
int func_089DDB20(unsigned char *object);
void func_089DDE4C(unsigned char *object);
void func_089E5AA8(BattleControlState *state);
int func_089E5BDC(BattleControlState *state);
int func_089E5C28(BattleControlState *state);
int func_089E5C60(BattleControlState *state);
int func_089E5C88(BattleControlState *state);
int func_089DDA7C(unsigned char *object);
int func_089DDB70(unsigned char *object);
int func_089E5644(unsigned char *object);
int func_089E5660(unsigned char *object);
int func_089E567C(unsigned char *object);
int func_089E5698(unsigned char *object);
int func_089E56B4(unsigned char *object);
int func_089E56D0(unsigned char *object);
int func_089E6B30(unsigned char *object);
int func_089E6B4C(unsigned char *object);
int func_089E6CAC(unsigned char *object);
int func_089E6CC8(unsigned char *object);
void func_089E69C0(unsigned char *object);


/* Partial views for the member-table stores in the setup constructors. */
typedef struct BattleTableAt94 { unsigned char padding[148]; void *member_table; } BattleTableAt94;
typedef struct BattleTableAt8 { unsigned char padding[8]; void *member_table; } BattleTableAt8;
typedef struct BattleTableAt1C0 { unsigned char padding[448]; void *member_table; } BattleTableAt1C0;
typedef struct BattleTableAt11DC { unsigned char padding[4572]; void *member_table; } BattleTableAt11DC;
typedef struct BattleTableAt44 { unsigned char padding[68]; void *member_table; } BattleTableAt44;
typedef struct BattleTableAt864 { unsigned char padding[2148]; void *member_table; } BattleTableAt864;
typedef struct BattleTableAt4 { unsigned char padding[4]; void *member_table; } BattleTableAt4;
typedef struct BattleTableAt20 { unsigned char padding[32]; void *member_table; } BattleTableAt20;
typedef struct BattleResetPrefix { unsigned char field_00; unsigned char padding_01[3]; void *field_04; } BattleResetPrefix;

void *func_089E06A8(unsigned char *object);
void *func_089E0CB8(unsigned char *object);
void func_089E07A0(unsigned char *object);
void *func_089E3208(unsigned char *object);
void *func_089E68E0(unsigned char *object);
void *func_089E6E60(unsigned char *object);
void *func_089E7130(unsigned char *object);
void func_089E71F8(unsigned char *object);
void *func_089E79A8(unsigned char *object);
void *func_089E8E70(unsigned char *object);
void func_089E8EDC(unsigned int *object);
void *func_089ECF40(unsigned char *object);

/* Pointer fields forwarded to cleanup helpers; complete ownership is unresolved. */
typedef struct BattleResourcePrefix { unsigned char padding[4]; void *field_04; } BattleResourcePrefix;
typedef struct BattleResource64C { unsigned char padding[0x64C]; void *field_64C; } BattleResource64C;

void func_089E0708(unsigned char *object, int flags);
void func_089E07AC(unsigned char *object);
void func_089E3258(unsigned char *object, int flags);
void func_089E6928(unsigned char *object, int flags);
void func_089E7180(unsigned char *object, int flags);
void func_089E7A20(unsigned char *object, int flags);
void func_089E6E98(unsigned char *object, int flags);
void func_089ECF78(unsigned char *object, int flags);
void func_089ED01C(unsigned char *object);
void func_089E6EFC(unsigned char *object);
void func_089E0D48(unsigned char *object);
void func_089E6EF0(unsigned char *object);

/* Three flag bytes accessed by the bounded slot-update helpers. */
typedef struct BattleFlagSlots {
    unsigned char field_00;
    unsigned char padding_01[0x1B7];
    unsigned char field_1B8[3];
} BattleFlagSlots;

int func_089DE360(unsigned char *object);
unsigned char func_089E0B9C(unsigned char *object);
int func_089E0DC0(unsigned char *object);
int func_089E3768(unsigned char *object, unsigned int index);
int func_089E37C0(unsigned char *object, unsigned int index);
int func_089E3B0C(unsigned char *object);
int func_089E3C3C(unsigned char *object);
int func_089E5598(unsigned char *object);
int func_089E69F8(unsigned char *object, void *field_a, void *field_b);
int func_089E6A74(unsigned char *object);
int func_089EA064(unsigned char *object);

/* Partial fields touched by the connected activation/binding routines. */
typedef struct BattleActivationHeader {
    unsigned char field_00;
    unsigned char padding_01[3];
    void *field_04;
    void *field_08;
    unsigned char padding_0C[0xC];
    void *field_18;
} BattleActivationHeader;
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
struct BattleDrawingStep;

int func_089E1EC0(unsigned char *object, void *input);
int func_089E2B48(unsigned char *object, unsigned int index, void *input);
int func_089E20FC(unsigned char *object, unsigned int index);
int func_089E2164(unsigned char *object, unsigned int index);
int func_089EA0AC(unsigned char *object, int value);
unsigned char func_089EA11C(unsigned char *object);
int func_089E5988(unsigned char *object);
int func_089E4368(unsigned char *object);
void func_089E9EF4(unsigned char *object);
int func_089E56EC(struct BattleDrawingStep *state);

/* Bound slot lookup and subsequent binding/update views. These are partial. */
struct BattleLookupState;
typedef struct BattleLookupState BattleLookupState;
typedef struct BattleLookupController {
    unsigned char padding[0x38];
    BattleLookupState *field_38[2];
} BattleLookupController;
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
typedef struct BattleBindingB944 {
    unsigned char field_00;
    unsigned char padding_01[3];
    unsigned int field_04;
    void *field_08;
} BattleBindingB944;
typedef struct BattleRelease7450 {
    unsigned char padding_00[8];
    void *field_08;
    void *field_0C;
} BattleRelease7450;
typedef struct BattleScale18 { unsigned char padding[0x18]; float field_18; } BattleScale18;
typedef struct BattleSlotIndexPrefix {
    unsigned char field_00;
    unsigned char padding_01[3];
    unsigned int field_04;
} BattleSlotIndexPrefix;

BattleLookupState *func_089E95B0(unsigned int index);
int func_089EA7B0(unsigned char *object, void *input, unsigned int index, void *shared, void *dependency);
int func_089EB944(unsigned char *object, unsigned int index, void *input);
int func_089E5B60(BattleControlState *state);
int func_089E63D4(unsigned char *object);
int func_089E3BF0(unsigned char *object);
int func_089E4A3C(unsigned char *object);
void func_089E7450(BattleRelease7450 *state);
int func_089E0904(unsigned char *object);
int func_089E1FD0(unsigned char *object);
int func_089E3618(unsigned char *object);
int func_089E7318(unsigned char *object);
int func_089E7F2C(unsigned char *object);

/* Original callee consumes only a bounded unsigned slot index (0 or 1). */
int func_089E96DC(unsigned int index);

/* Signed status returns are observed even when the caller ignores them. */
int func_089E132C(void *object);
int func_089E2BB4(void *object);
int func_089E2418(void *object);

#endif
