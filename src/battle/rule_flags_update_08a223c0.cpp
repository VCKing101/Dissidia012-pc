/* Reads each side's rule entry and sets the fighter's flag from it. */
struct RuleEntry {
    unsigned char data[0x50];
};

struct RuleInfo {
    unsigned char pad_00[0xB];
    unsigned char enabled;
    unsigned char pad_0c[0x4C];
};

struct FlagUnit {
    unsigned char pad_0000[0x12AA];
    unsigned char flag;

    static void set(FlagUnit *unit, unsigned char on) { unit->flag = on; }
};

struct RuleFlags {
    unsigned char open;
    unsigned char pad_01[3];
    unsigned char list[0x484];
    RuleEntry *entries;
};

extern "C" {

extern void func_08A233AC(void *list);
extern FlagUnit *func_08A22C78(int side);
extern int func_088916A4(RuleEntry *entry, RuleInfo *info);

int func_08A223C0(RuleFlags *flags)
{
    if (!flags->open) {
        return -1;
    }
    func_08A233AC(flags->list);
    for (int side = 0; side < 2; side++) {
        FlagUnit *unit = func_08A22C78(side);
        if (unit == 0) {
            continue;
        }
        RuleInfo info;
        if (func_088916A4(&flags->entries[side], &info) != 0) {
            continue;
        }
        FlagUnit::set(unit, info.enabled != 0);
    }
    return 0;
}

}
