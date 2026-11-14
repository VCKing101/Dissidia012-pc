typedef struct SoundSystem {
    void *state;
    char pad_04[0xC - 4];
    unsigned char locked;
} SoundSystem;

extern SoundSystem D_089D7F00;
extern void func_089C58A8(int resource);

void func_088ECF90(void)
{
    SoundSystem *sys = &D_089D7F00;

    if (sys->locked == 0) {
        return;
    }
    func_089C58A8(0x303);
    sys->locked = 0;
}
