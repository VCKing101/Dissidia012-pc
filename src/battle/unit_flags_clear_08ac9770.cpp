typedef struct UnitFlags {
    char pad_00[0x7];
    unsigned char bits;

    static void clear(UnitFlags *flags, int mask) { flags->bits &= ~mask; }
} UnitFlags;

typedef struct FlagUnit {
    char pad_000[0x5B0];
    UnitFlags flags;
} FlagUnit;

extern "C" int func_08AC9770(void *self, FlagUnit *unit)
{
    if (unit != 0) {
        UnitFlags *flags = &unit->flags;
        UnitFlags::clear(flags, 1);
        UnitFlags::clear(flags, 4);
    }
    return 0;
}
