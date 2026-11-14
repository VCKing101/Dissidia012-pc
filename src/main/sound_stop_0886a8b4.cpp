/* Clears the pending bit and, when playing, stops the voice. */
struct Voice0886A8B4 {
    int field_00;
    int field_04;
    int handle;
};

struct Sound0886A8B4 {
    char pad_00[0x84];
    Voice0886A8B4 *voice;
    int field_88;
    int field_8C;
    unsigned int flags;

    static bool playing(unsigned int flags) { return (flags & 0x20) != 0; }
};

extern "C" {

void func_089B0BAC(int handle);

void func_0886A8B4(Sound0886A8B4 *s)
{
    unsigned int flags = s->flags & ~0x40;
    s->flags = flags;
    if (!Sound0886A8B4::playing(flags)) {
        return;
    }
    s->flags = flags & ~0x20;
    func_089B0BAC(s->voice->handle);
}

}
