typedef struct UnitKind {
    int kind;
} UnitKind;

typedef struct KindUnit {
    char pad_000[0x3A4];
    UnitKind *info;
} KindUnit;

int func_08A83CBC(KindUnit *unit)
{
    if (unit == 0) {
        return 0;
    }
    if (unit->info == 0) {
        return 0;
    }
    return unit->info->kind == 13;
}
