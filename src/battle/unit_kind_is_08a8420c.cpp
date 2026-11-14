typedef struct UnitKind {
    int kind;
} UnitKind;

typedef struct KindUnit {
    char pad_000[0x3A4];
    UnitKind *info;
} KindUnit;

extern "C" bool func_08A8420C(KindUnit *unit)
{
    if (unit == 0) {
        return false;
    }
    int kind = unit->info->kind;
    return kind == 19 || kind == 14 || kind == 13;
}
