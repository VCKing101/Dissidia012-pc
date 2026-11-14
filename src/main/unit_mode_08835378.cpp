/* Sets mode 11 on or off from bit 5 of the status byte. */
struct Status08835378 {
    char pad_0[6];
    unsigned char bits;

    static bool flag(Status08835378 *s) { return (s->bits & 0x20) != 0; }
};

struct Unit08835378 {
    char pad_000[0x190];
    Status08835378 status;
};

extern "C" {

void func_088352E8(Unit08835378 *u, int mode, int on);

void func_08835378(Unit08835378 *u)
{
    Status08835378 *status = &u->status;
    if (Status08835378::flag(status)) {
        func_088352E8(u, 11, 1);
    } else {
        func_088352E8(u, 11, 0);
    }
}

}
