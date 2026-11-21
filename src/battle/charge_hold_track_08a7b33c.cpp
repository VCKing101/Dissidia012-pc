/* Charge-hold tracker: while the fighter holds a charge (status bit) below
 * level 10000, counts the frames the same charge level is kept and flags
 * a full hold after 60 frames; otherwise the count restarts (-1 closed, -2
 * no fighter). */
struct ChargeLevel {
    unsigned char pad_00[0x18];
    float level;
};

struct ChargeStatus {
    unsigned char pad_0[9];
    unsigned char b9;
};

struct ChargeFighter {
    unsigned char pad_000[0x190];
    ChargeStatus status;
    unsigned char pad_19a[0x392];
    ChargeLevel *charge;

    static bool charging(ChargeFighter *fighter)
    {
        ChargeStatus *status = &fighter->status;
        return (status->b9 & 2) != 0;
    }
};

struct ChargeOwner {
    unsigned char pad_00[4];
    ChargeFighter *fighter;
};

struct ChargeTrack {
    unsigned char open;
    unsigned char pad_01[3];
    ChargeOwner *owner;
    unsigned char pad_08[0xC];
    unsigned char full;
    unsigned char pad_15[3];
    int level;
    float time;
};

extern "C" {

extern void *func_08B30898(ChargeFighter *fighter);
extern float func_08838300(ChargeFighter *fighter);

int func_08A7B33C(ChargeTrack *self)
{
    if (!self->open) {
        return -1;
    }
    ChargeFighter *fighter = self->owner->fighter;
    if (fighter == 0) {
        return -2;
    }
    int level = (int)fighter->charge->level;
    func_08B30898(fighter);
    if (ChargeFighter::charging(fighter) == 1 && level < 10000) {
        if (self->time == 0.0f) {
            self->level = level;
        }
        if (self->level != level) {
            self->time = 0.0f;
        } else {
            self->time = self->time + func_08838300(fighter);
            if (!(self->time < 60.0f)) {
                self->full = 1;
            }
        }
    } else {
        self->time = 0.0f;
    }
    return 0;
}

}
