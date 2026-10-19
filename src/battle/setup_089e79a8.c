#include "battle/runtime_helpers.h"

/* Constructor-like setup path; names and complete class layouts remain unknown. */
extern unsigned char D_089E8228[];
extern unsigned char D_089E5218[];
extern unsigned char D_089E8F68[];
extern void func_08993560(void *, unsigned int, unsigned int, void *);
extern void func_089DDFC8(void *);
extern void func_089E7AD0(void *);

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
