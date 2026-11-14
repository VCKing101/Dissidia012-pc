/* Clears the pending bit and, when playing, pauses the voice and remembers it. */
struct Voice0886A948 {
    int field_00;
    int field_04;
    int handle;
};

struct Sound0886A948 {
    char pad_00[0x84];
    Voice0886A948 *voice;
    int field_88;
    int field_8C;
    unsigned int flags;

    static bool playing(unsigned int flags) { return (flags & 0x20) != 0; }
};

extern "C" {

void func_089B09E8(int handle);

void func_0886A948(Sound0886A948 *s)
{
    unsigned int flags = s->flags & ~0x40;
    s->flags = flags;
    if (!Sound0886A948::playing(flags)) {
        return;
    }
    s->flags = flags & ~0x20;
    func_089B09E8(s->voice->handle);
    s->field_88 = (int)s->voice;
}

}
