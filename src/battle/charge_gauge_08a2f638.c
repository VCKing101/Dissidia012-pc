typedef struct ChargeStatus {
    int active;
    int phase;
    float charge;
    char pad_0C[0x14];
    int held;
    float limit;
} ChargeStatus;

typedef struct ChargeOwner {
    char pad_000[0x5C4];
    int gauge;
} ChargeOwner;

typedef struct ChargeCheck {
    ChargeOwner *owner;
    char pad_04[0x10];
    ChargeStatus *status;
} ChargeCheck;

extern void func_08A6427C(int gauge, int on);

/* Lights the gauge unless the charge is still building or was released early. */
void func_08A2F638(ChargeCheck *self)
{
    int show = 1;
    ChargeStatus *status = self->status;
    if (status->active != 1) {
        return;
    }
    if (status->phase == 3 && status->held == 1 && status->charge > status->limit) {
        show = 0;
    }
    if (status->phase == 4 && status->held == 0) {
        show = 0;
    }
    if (show) {
        func_08A6427C(self->owner->gauge, 1);
    }
}
