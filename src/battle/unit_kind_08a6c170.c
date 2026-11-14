typedef struct UnitKind6C170 {
    char pad_000[0xE];
    unsigned char kind;
} UnitKind6C170;

typedef struct UnitOwner6C170 {
    char pad_000[0x64];
    int field_64;
} UnitOwner6C170;

extern UnitKind6C170 *func_08A6B038(UnitOwner6C170 *self);

int func_08A6C170(UnitOwner6C170 *self)
{
    UnitKind6C170 *unit = func_08A6B038(self);
    if (!unit) {
        return 0;
    }
    return unit->kind == 6;
}

int func_08A6C1AC(UnitOwner6C170 *self)
{
    UnitKind6C170 *unit = func_08A6B038(self);
    if (!unit) {
        return 0;
    }
    return unit->kind == 7;
}

int func_08A6C1E8(UnitOwner6C170 *self)
{
    UnitKind6C170 *unit = func_08A6B038(self);
    if (!unit) {
        return 0;
    }
    return unit->kind == 8;
}

int func_08A6C224(UnitOwner6C170 *self)
{
    UnitKind6C170 *unit = func_08A6B038(self);
    if (!unit) {
        return 0;
    }
    return unit->kind == 9;
}

int func_08A6C260(UnitOwner6C170 *self)
{
    UnitKind6C170 *unit = func_08A6B038(self);
    if (!unit) {
        return 0;
    }
    return unit->kind == 10;
}
