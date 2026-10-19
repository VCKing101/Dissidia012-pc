#include "battle/runtime_helpers.h"

/* Connected activation/state helpers; complete class ownership is unknown. */
extern void *D_089D7830;
extern void *D_089D6998;
extern void func_089E1E30(void *);
extern void *func_088D09D8(void *, int, int);
extern void *func_0887E0CC(void *, int);
extern int func_089E2304(void *, unsigned int, void *);

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
