typedef struct UnitKind {
    int kind;
} UnitKind;

typedef struct KindUnit {
    char pad_000[0x3A4];
    UnitKind *info;
} KindUnit;

extern int func_08A59294(KindUnit *unit);

int func_08A83FDC(KindUnit *unit)
{
    if (unit == 0) {
        return 0;
    }
    if (unit->info->kind != 17) {
        return 0;
    }
    return func_08A59294(unit);
}
