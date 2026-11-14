struct UnitFlags5B0 {
    unsigned char bytes[4];

    static bool hasBit40(const UnitFlags5B0 &flags) { return (flags.bytes[2] & 0x40) != 0; }
};

struct Unit75BC {
    char pad_000[0x5B0];
    UnitFlags5B0 flags;
};

struct Watcher75BC {
    char pad_00[0x4];
    int triggered;
    char pad_08[0x8];
    int unit_id;
};

extern "C" {

extern Unit75BC *func_089E95B0(int id);

int func_089E75BC(Watcher75BC *self)
{
    Unit75BC *unit = func_089E95B0(self->unit_id);
    if (unit == 0) {
        return -1;
    }
    if (!UnitFlags5B0::hasBit40(unit->flags)) {
        return 0;
    }
    self->triggered = 1;
    return 0;
}

}
