typedef struct HoldStatus {
    char pad_0[0x3];
    unsigned char flags;
} HoldStatus;

typedef struct HoldUnit {
    char pad_000[0x380];
    unsigned int flags;
    char pad_384[0x22C];
    HoldStatus status;
    char pad_5B4[0xB18];
    short motion;
} HoldUnit;

typedef struct HoldRelease {
    char pad_00[0x4];
    int holding;
    float elapsed;
    char pad_0C[0x24];
    void *field_30;
    void *field_34;
    void *field_38;
} HoldRelease;

extern void func_088A6AA4(void *handle);
extern void func_08AE3AD0(int motion);

void func_08AC485C(HoldRelease *self, HoldUnit *unit)
{
    HoldStatus *status;
    if (self->field_30 != 0) {
        func_088A6AA4(self->field_30);
    }
    self->field_30 = 0;
    if (self->field_34 != 0) {
        func_088A6AA4(self->field_34);
    }
    self->field_34 = 0;
    if (self->field_38 != 0) {
        func_088A6AA4(self->field_38);
    }
    self->field_38 = 0;
    if (self->holding != 0) {
        unit->flags &= ~0x80000;
    }
    status = &unit->status;
    status->flags &= ~4;
    status->flags &= ~1;
    status->flags &= ~2;
    func_08AE3AD0(unit->motion);
    {
        float zero = 0.0f;
        self->holding = 0;
        self->elapsed = zero;
    }
}
