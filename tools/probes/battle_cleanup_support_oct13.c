#include "battle/runtime_helpers.h"

typedef struct BattleOct13ResetView {
    unsigned char field_00;
    unsigned char padding_01[3];
    void *field_04;
    int field_08;
    int field_0C;
    void *field_10;
    void *field_14;
} BattleOct13ResetView;
typedef struct BattleOct13BufferView {
    unsigned char field_00;
    unsigned char padding_01[3];
    int field_04;
    unsigned char padding_08[0x9C];
    unsigned char field_A4[0x30];
} BattleOct13BufferView;

extern void *func_08935828(void *, int, unsigned int);
extern int func_089E439C(void *);
extern void func_089E3F98(void *);
extern void func_089E4A94(void *);
extern void func_089E4550(void *);

void func_089E1248(void *object)
{
    BattleOct13ResetView *state = (BattleOct13ResetView *)object;
    state->field_00 = 0;
    state->field_04 = 0;
    state->field_08 = -1;
    state->field_0C = -1;
    state->field_10 = 0;
    state->field_14 = 0;
}

void func_089E399C(void *object)
{
    BattleOct13BufferView *state = (BattleOct13BufferView *)object;
    state->field_00 = 0;
    state->field_04 = -1;
    func_08935828(state->field_A4, 0, 0x30);
}

void func_089E3FE4(void *object)
{
    func_089E439C(object);
    func_089E3F98(object);
}

void func_089E4594(void *object)
{
    func_089E4A94(object);
    func_089E4550(object);
}
