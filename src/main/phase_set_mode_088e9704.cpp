/* Stores the mode; mode 1 clears the global pause flag, mode 2 sets it and resets the pad. */
struct PauseFlags {
    bool paused;
};

struct PhaseMode {
    char pad_00[0x2C];
    int mode;
    int mode_copy;
};

extern "C" {
extern PauseFlags *D_089D69E0;
extern void *D_089D7830;
extern void func_088CF6CC(void *pad, int mask);
extern void func_088CF750(void *pad, int value);
}

struct Pause {
    static void set(bool paused)
    {
        D_089D69E0->paused = paused;
    }

    static void resetPad(void)
    {
        int zero = 0;

        func_088CF750(D_089D7830, zero);
    }
};

extern "C" {
void func_088E9704(PhaseMode *owner, int mode)
{
    owner->mode = mode;
    owner->mode_copy = mode;
    switch (mode) {
    case 1:
        Pause::set(false);
        func_088CF6CC(D_089D7830, 0xFF);
        break;
    case 2:
        Pause::set(true);
        func_088CF6CC(D_089D7830, 2);
        Pause::resetPad();
        break;
    }
}
}
