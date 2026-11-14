/* 1 unless flag 0x20 is set and x is negative. */
struct Obj08847EC4 {
    char pad_00[0xF];
    unsigned char bits;

    static bool flag(Obj08847EC4 *o) { return (o->bits & 0x20) != 0; }
};

extern "C" {

unsigned char func_08847EC4(Obj08847EC4 *o, float x)
{
    if (!Obj08847EC4::flag(o)) {
        return 1;
    }
    int result = 0;
    if (!(x < 0.0f)) {
        result = 1;
    }
    return result;
}

}
