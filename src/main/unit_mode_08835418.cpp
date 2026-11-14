/* Sets mode 10 on or off from bit 3 of the status byte. */
struct Status08835418 {
    char pad_0[6];
    unsigned char bits;

    static bool flag(Status08835418 *s) { return (s->bits & 0x8) != 0; }
};

struct Unit08835418 {
    char pad_000[0x190];
    Status08835418 status;
};

extern "C" {

void func_088352E8(Unit08835418 *u, int mode, int on);

void func_08835418(Unit08835418 *u)
{
    Status08835418 *status = &u->status;
    if (Status08835418::flag(status)) {
        func_088352E8(u, 10, 1);
    } else {
        func_088352E8(u, 10, 0);
    }
}

}
