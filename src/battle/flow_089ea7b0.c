#include "battle/runtime_helpers.h"

/* Connected slot/binding/update flow; full class ownership remains unknown. */
extern void func_089EA74C(void *);

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
