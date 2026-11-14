typedef struct BonusValue {
    short percent;
} BonusValue;

typedef struct BonusMod {
    char pad_00[0x4];
    BonusValue value;
} BonusMod;

typedef struct BonusItem {
    char pad_00[0x4];
    short base;
} BonusItem;

extern int func_08835798(int unit);
extern BonusMod *func_088BF4CC(int owner, int id);

int func_08ADE14C(void *self, BonusItem *item, int unit, int base, int bonus)
{
    int total;
    BonusMod *mod;
    BonusValue *value;
    if (item != 0) {
        total = item->base + base + bonus;
        mod = func_088BF4CC(func_08835798(unit), 0x103);
        if (mod != 0) {
            value = &mod->value;
            total = total * (value->percent + 100) / 100;
        }
        return total;
    }
    return 0;
}
