extern "C" {
struct Unit08839050 { char pad_000[0x108]; void *owner; char pad_10C[0x120 - 0x10C]; Unit08839050 *next; };
extern signed char *D_089D85E8;
extern Unit08839050 *D_09AFB890;
struct UnitOps {
    static bool isOne() { return *D_089D85E8 == 1; }
    static Unit08839050 *after(Unit08839050 *u) { return u == 0 ? D_09AFB890 : u->next; }
};
bool func_08839050(void *owner)
{
    if (!UnitOps::isOne()) {
        return false;
    }
    Unit08839050 *u = 0;
    while ((u = UnitOps::after(u)) != 0) {
        if (u->owner == owner) {
            return true;
        }
    }
    return false;
}
}
