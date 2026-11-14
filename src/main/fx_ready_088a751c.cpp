/* 1 when the effect has a voice whose counted bit is set. */
struct Voice088A751C {
    char pad_00[0x18];
    unsigned int flags;

    static unsigned char counted(Voice088A751C *v) { return v->flags & 1; }
};

struct Fx088A751C {
    char pad_00[0xCC];
    Voice088A751C *voice;
};

extern "C" {

unsigned char func_088A751C(Fx088A751C *fx)
{
    int result = 0;
    if (fx->voice != 0 && Voice088A751C::counted(fx->voice) == 1) {
        result = 1;
    }
    return result;
}

}
