/* Without a second slot falls back to func_088E91CC; with menu state 6 restarts the menu. */
struct PhaseOwner {
    char pad_0000[0x90];
    int menu_state;
    int field_94;
    int second;

    static bool isSet(int value)
    {
        return value != 0;
    }

    static bool is(int value, int wanted)
    {
        return value == wanted;
    }
};

extern "C" {
extern void func_088E01C4(void *menu, int arg);
extern void func_088E91CC(PhaseOwner *owner);

void func_088E9390(PhaseOwner *owner)
{
    if (PhaseOwner::isSet(owner->second)) {
        if (PhaseOwner::is(owner->menu_state, 6)) {
            func_088E01C4(&owner->menu_state, 0);
        }
    } else {
        func_088E91CC(owner);
    }
}
}
