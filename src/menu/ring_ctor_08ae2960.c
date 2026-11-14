typedef struct RingBody {
    char pad_000[0x594];
    int field_594;
    int field_598;
    int field_59C;
} RingBody;

typedef struct RingScreen {
    void *field_00;
    char field_04[0x84C];
    RingBody *field_850;
} RingScreen;

extern int D_08AE4550;
extern void func_08AF1230(RingScreen *self);

RingScreen *func_08AE2960(RingScreen *self)
{
    RingBody *body;
    func_08AF1230(self);
    self->field_00 = &D_08AE4550;
    body = (RingBody *)self->field_04;
    self->field_850 = body;
    body->field_594 = 0;
    self->field_850->field_598 = 0;
    self->field_850->field_59C = 0;
    return self;
}
