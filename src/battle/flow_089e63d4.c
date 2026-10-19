#include "battle/runtime_helpers.h"

/* Connected slot/binding/update flow; full class ownership remains unknown. */
extern void func_089E6450(void *, int);
extern int func_089E6594(void *, int);

int func_089E63D4(unsigned char *object)
{
    int current;
    if (!object[0]) return -1;
    current = func_089E96DC(((BattleSlotIndexPrefix *)object)->field_04);
    func_089E6450(object, current);
    if (func_089E6594(object, current)) return -2;
    return 0;
}
