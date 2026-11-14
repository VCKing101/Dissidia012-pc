extern void func_089A34E0(void *vec);
extern unsigned long long D_089CE280;
extern unsigned long long D_089CE290;

typedef struct Unit {
    char pad_000[0x240];
    short field_240;
    short field_242;
    char vec_244[0xC];
    char vec_250[0x10];
    unsigned long long flags;   /* 0x260 */
} Unit;

void func_089127C4(Unit *unit)
{
    unit->field_240 = -1;
    unit->field_242 = -1;
    func_089A34E0(unit->vec_244);
    func_089A34E0(unit->vec_250);
    unit->flags = unit->flags & D_089CE280 & D_089CE290;
}
