/* In mode 3: false while any unit is busy, otherwise true when a unit carries
 * one of the pause flags. Mode 4 always allows it. */
struct BattleSession {
    unsigned char pad_000[0x48];
    int mode;

    static bool is(BattleSession *session, int mode) { return session->mode == mode; }
};

struct UnitAction {
    unsigned char data[4];
};

struct FlagUnit {
    unsigned char pad_000[0x388];
    unsigned long long flags;
    unsigned char pad_390[0x3F4];
    UnitAction action;

    static bool any(FlagUnit *unit, unsigned long long mask) { return (unit->flags & mask) != 0; }
};

extern "C" {

extern BattleSession *D_089D6A10;
extern unsigned long long D_089E9E08;
extern FlagUnit *func_08841FA4(FlagUnit *after);
extern int func_08ACAA74(UnitAction *action);
extern int func_08ACAA9C(UnitAction *action);

bool func_089E9C64(void)
{
    if (BattleSession::is(D_089D6A10, 3) == 1) {
        FlagUnit *unit = 0;
        while ((unit = func_08841FA4(unit)) != 0) {
            UnitAction *action = &unit->action;
            if (func_08ACAA74(action) == 0 && func_08ACAA9C(action) == 0) {
                continue;
            }
            return false;
        }
        unit = 0;
        while ((unit = func_08841FA4(unit)) != 0) {
            if (FlagUnit::any(unit, D_089E9E08)) {
                return true;
            }
        }
        return false;
    }
    if (BattleSession::is(D_089D6A10, 4) == 1) {
        return true;
    }
    return false;
}

}
