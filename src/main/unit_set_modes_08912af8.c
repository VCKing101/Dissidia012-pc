typedef struct UnitModes {
    char pad_000[0xF];
    unsigned char mode;         /* 0x00F */
    char pad_010[0x1D0 - 0x10];
    unsigned char subMode;      /* 0x1D0 */
} UnitModes;

void func_08912AF8(UnitModes *unit, int mode, int subMode)
{
    unit->mode = mode;
    unit->subMode = subMode;
}
