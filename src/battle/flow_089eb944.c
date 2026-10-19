#include "battle/runtime_helpers.h"

/* Connected slot/binding/update flow; full class ownership remains unknown. */
extern void func_089EB904(void *);
extern void *func_088DAA40(void *, int, int);

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
