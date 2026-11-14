/* Caps the volume at 1 while the state plays (modes 1 and 3). */
struct State08819140 {
    char pad_00[0xC];
    unsigned char active;
    char pad_0D[3];
    int mode;
    char pad_14[0x14];
    float volume;
};

extern "C" {

int func_08819140(State08819140 *s)
{
    if (s->active == 0) {
        return -1;
    }
    if (s->mode == 1 || s->mode == 3) {
        if (!(s->volume <= 1.0f)) {
            s->volume = 1.0f;
        }
    }
    return 0;
}

}
