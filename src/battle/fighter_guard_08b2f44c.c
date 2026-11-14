typedef struct FighterGuard {
    char pad000[0xD64];
    char guard[4];          /* 0xD64 */
} FighterGuard;

extern int func_0884F878(void *guard);
extern int func_08834248(FighterGuard *fighter);

int func_08B2F44C(FighterGuard *fighter)
{
    if (func_0884F878(fighter->guard) != 0) {
        return 1;
    }
    if (func_08834248(fighter) != 0) {
        return 1;
    }
    return 0;
}
