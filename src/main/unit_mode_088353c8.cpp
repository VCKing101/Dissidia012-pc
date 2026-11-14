/* Sets mode 15 on or off from bit 4 of the status byte. */
struct Status088353C8 {
    char pad_0[6];
    unsigned char bits;

    static bool flag(Status088353C8 *s) { return (s->bits & 0x10) != 0; }
};

struct Unit088353C8 {
    char pad_000[0x190];
    Status088353C8 status;
};

extern "C" {

void func_088352E8(Unit088353C8 *u, int mode, int on);

void func_088353C8(Unit088353C8 *u)
{
    Status088353C8 *status = &u->status;
    if (Status088353C8::flag(status)) {
        func_088352E8(u, 15, 1);
    } else {
        func_088352E8(u, 15, 0);
    }
}

}
