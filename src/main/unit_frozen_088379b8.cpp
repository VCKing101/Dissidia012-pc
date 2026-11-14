/* 1 when flag 0x100000 or status bit 0x4 of byte 2 is set. */
struct Status088379B8 {
    unsigned char field_0;
    unsigned char field_1;
    unsigned char bits;

    static bool flag(Status088379B8 *s) { return (s->bits & 4) != 0; }
};

struct Unit088379B8 {
    char pad_000[0x190];
    Status088379B8 status;
    char pad_193[0x1ED];
    unsigned int flags;

    static bool has(Unit088379B8 *u, unsigned int mask) { return (u->flags & mask) != 0; }
};

extern "C" {

unsigned char func_088379B8(Unit088379B8 *u)
{
    Status088379B8 *status;
    int result = 0;
    if (Unit088379B8::has(u, 0x100000) || (status = &u->status, Status088379B8::flag(status))) {
        result = 1;
    }
    return result;
}

}
