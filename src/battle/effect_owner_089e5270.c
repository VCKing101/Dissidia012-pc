#include "battle/agent1_types.h"

/* Destructor, reset and release of the 0xDC-byte effect owner. */
extern unsigned char D_089E59D0[];
extern void func_089ED8D0(void *, int);
extern void func_089E4BC8(BattleEffectSlot *, int);
extern void func_089E5A50(void *, int);
extern void func_089E60E0(void *, int);
extern void func_089ED974(void *);
extern void func_089E4C50(BattleEffectSlot *);
extern void func_089E5AD0(void *);
extern void func_089E6184(void *);
extern void func_088DA060(void *);
extern void func_08992F04(void *);
void func_089E52F8(BattleEffectOwner *owner);
void func_089E5314(BattleEffectOwner *owner);

void func_089E5270(BattleEffectOwner *owner, int flags)
{
    if (owner) {
        owner->vtable = D_089E59D0;
        func_089E5314(owner);
        func_089ED8D0(owner->field_84, 2);
        func_089E4BC8(&owner->field_58, 2);
        func_089E5A50(owner->field_40, 2);
        func_089E60E0(owner->field_10, 2);
        if (flags & 1) func_08992F04(owner);
    }
}

void func_089E52F8(BattleEffectOwner *owner)
{
    owner->field_00 = 0;
    owner->field_04 = 0;
    owner->field_08 = 0;
    owner->field_0C = 0;
    owner->field_D0 = 0;
    owner->field_D4 = 0;
}

void func_089E5314(BattleEffectOwner *owner)
{
    func_089ED974(owner->field_84);
    func_089E4C50(&owner->field_58);
    if (owner->field_D4) {
        func_088DA060(owner->field_D4);
        owner->field_D4 = 0;
    }
    if (owner->field_D0) {
        func_088DA060(owner->field_D0);
        owner->field_D0 = 0;
    }
    func_089E5AD0(owner->field_40);
    func_089E6184(owner->field_10);
    func_089E52F8(owner);
}
