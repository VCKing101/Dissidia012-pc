#include "battle/runtime_helpers.h"
typedef struct BattleInput1C4 { unsigned char padding[0x1C4]; void *field_1C4; } BattleInput1C4;
extern void func_089E9F04(void *);
extern void *func_088DAA40(void *, int, int);
extern int func_089EA7B0(void *, void *, unsigned int, void *, void *);
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
        shared = object + 8;
        entry = shared + 0x14;
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
