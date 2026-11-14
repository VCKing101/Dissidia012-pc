extern "C" {
extern void func_08912A04(void *unit, int index);
extern unsigned long long D_089CE270;

typedef struct Unit {
    char pad_000[0x260];
    unsigned long long flags;   /* 0x260 */
} Unit;

void func_08912A6C(Unit *unit)
{
    for (int i = 0; i < 4; i++) {
        func_08912A04(unit, i);
    }
    unit->flags = unit->flags & D_089CE270;
}
}
