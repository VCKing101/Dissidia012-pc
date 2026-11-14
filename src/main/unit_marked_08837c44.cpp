/* 1 when the unit's mark is active or status bit 0x20 of byte 4 is set. */
struct Status08837C44 {
    char pad_0[4];
    unsigned char bits;

    static bool flag(Status08837C44 *s) { return (s->bits & 0x20) != 0; }
};

struct Unit08837C44 {
    char pad_000[0x190];
    Status08837C44 status;
    char pad_195[0x207];
    void *mark;
};

extern "C" {

int func_0884E53C(void *mark);

unsigned char func_08837C44(Unit08837C44 *u)
{
    Status08837C44 *status;
    return ((u->mark != 0 && func_0884E53C(u->mark) != 0) ||
            (status = &u->status, Status08837C44::flag(status))) ? 1 : 0;
}

}
