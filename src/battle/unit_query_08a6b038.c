typedef struct UnitSlot6B038 {
    char pad_000[0x4];
    void *unit;
} UnitSlot6B038;

typedef struct UnitOwner6B038 {
    char pad_000[0x64];
    int field_64;
} UnitOwner6B038;

extern UnitSlot6B038 *func_08AB3264(int id);
extern void *func_08A973A8(UnitSlot6B038 *slot);
extern int func_08B30898(void *target);

int func_08A6B038(UnitOwner6B038 *self)
{
    void *target;
    if (!func_08AB3264(self->field_64)->unit) {
        return 0;
    }
    target = func_08A973A8(func_08AB3264(self->field_64));
    if (!target) {
        return 0;
    }
    return func_08B30898(target);
}
