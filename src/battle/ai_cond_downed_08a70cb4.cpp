/* AI condition: met unless the fighter is in state 0x15 and neither
 * downed (status bit 0) nor rising (flag 0x40000 without 0x10000000) (-1
 * bad arguments, -2 no fighter). */
struct DownState {
    int kind;
};

struct DownStatus {
    unsigned char pad_0[3];
    unsigned char b3;

    static unsigned char downed(DownStatus *status) { return status->b3 & 1; }
};

struct DownFighter {
    unsigned char pad_000[0x190];
    DownStatus status;
    unsigned char pad_194[0x1F0];
    unsigned int flags;
    unsigned char pad_388[0x1C];
    DownState *state;

    static bool has(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
    static unsigned char downed(DownFighter *fighter)
    {
        DownStatus *status = &fighter->status;
        return DownStatus::downed(status);
    }
};

struct DownInfo {
    unsigned char pad_00[4];
    DownFighter *fighter;
};

struct DownAi {
    unsigned char pad_00;
    unsigned char met;
    unsigned char pad_02[0x62];
    void *owner;
};

extern "C" {

extern DownInfo *func_08AB3264(void *owner);

int func_08A70CB4(DownAi *self, void *cond, void *key, void *flag)
{
    if (cond == 0 || key == 0 || flag == 0) {
        return -1;
    }
    DownFighter *fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -2;
    }
    if (fighter->state->kind != 0x15 || DownFighter::downed(fighter) == 1
        || (DownFighter::has(fighter->flags, 0x40000) == 1 && !DownFighter::has(fighter->flags, 0x10000000))) {
        self->met = 1;
    }
    return 0;
}

}
