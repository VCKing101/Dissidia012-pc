/* Takes the sound lock (resource 0x303) once. */
typedef struct SoundSystem {
    void *state;
    char pad_04[0xC - 4];
    unsigned char locked;
} SoundSystem;

extern SoundSystem D_089D7F00;
extern void func_089C5828(int resource);

void func_088ECF48(void)
{
    SoundSystem *sys = &D_089D7F00;

    if (sys->locked != 0) {
        return;
    }
    func_089C5828(0x303);
    sys->locked = 1;
}
