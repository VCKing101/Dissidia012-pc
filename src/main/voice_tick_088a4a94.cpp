/* Counts down the voice's loop time and advances its fade. */
struct VoiceBits088A4A94 {
    unsigned int field_0 : 1;
    unsigned int field_1 : 1;
    unsigned int field_2 : 1;
    unsigned int fading : 1;
};

struct Voice088A4A94 {
    char pad_00[0x18];
    VoiceBits088A4A94 bits;
    float remaining;

    static bool fading(Voice088A4A94 *v) { return v->bits.fading != 0; }
};

extern "C" {

void func_088A44F8(Voice088A4A94 *v, int loops);
void func_088A4B58(Voice088A4A94 *v, float dt);

void func_088A4A94(Voice088A4A94 *v, float dt)
{
    float remaining = v->remaining;
    if (!(remaining <= 0.0f)) {
        remaining -= dt;
        v->remaining = remaining;
        if (remaining <= 0.0f) {
            func_088A44F8(v, 0);
        }
    }
    if (Voice088A4A94::fading(v)) {
        func_088A4B58(v, dt);
    }
}

}
