#include "battle/agent28_types.h"

/* Puts the battle in mode 2 while the lock is held and back in mode 1 when
 * it is let go. */
struct BattleLock {
    unsigned char open;
    unsigned char held;
};

extern void func_08889944(RootHolder *battle, int speed);
extern RootHolder *D_089D6A10;

int func_08A15A94(struct BattleLock *lock)
{
    if (!lock->open) {
        return -1;
    }
    if (lock->held == 1) {
        return -2;
    }
    func_08889944(D_089D6A10, 2);
    lock->held = 1;
    return 0;
}

int func_08A15AF8(struct BattleLock *lock)
{
    if (!lock->open) {
        return -1;
    }
    if (!lock->held) {
        return -2;
    }
    func_08889944(D_089D6A10, 1);
    lock->held = 0;
    return 0;
}
