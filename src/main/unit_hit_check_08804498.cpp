extern "C" {
struct UnitFlags {
    unsigned char pad[3];
    unsigned char bits;
};
struct Unit {
    char pad0[0x130];
    char effect[0x50];
    short kind;
    char pad182[0xE];
    UnitFlags flags;
};
extern float D_089C6A18;
extern void func_0882FAC4(float scale, float delay, void *effect, int target, int mode);
extern int func_0883739C(Unit *unit);
extern int func_08B2F9BC(Unit *unit);
void func_08804498(int unused, Unit *unit, int target)
{
    short kind = unit->kind;
    unsigned char hit = 0;
    UnitFlags *flags;
    if (kind == 1 || kind == 2 || kind == 3) {
        hit = 1;
    }
    if (kind == 0x66 && func_0883739C(unit) != 0 && func_08B2F9BC(unit) != 0) {
        hit = 1;
    }
    if ((flags = &unit->flags, (unsigned char)(flags->bits & 1)) && func_0883739C(unit) != 0 && func_08B2F9BC(unit) != 0) {
        hit = 1;
    }
    if (hit) {
        func_0882FAC4(D_089C6A18, 0.0f, unit->effect, target, 0);
    }
}
}
