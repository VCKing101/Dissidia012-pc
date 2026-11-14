/* Runs one step of the screen's phase machine (phases 0, 100, 200, 300 and
   10000), then the common updates; returns the step's result. */
struct State08B2702C {
    char pad_000[0x1A8];
    int field_1A8;
    char pad_1AC[0x1B8 - 0x1AC];
    int field_1B8;
};

struct Owner08B2702C {
    struct State08B2702C *field_0;
};

extern void func_08B274C0(struct Owner08B2702C *self);
extern void func_08B27818(struct Owner08B2702C *self);
extern void func_08B253E4(struct Owner08B2702C *self);
extern void func_08B2716C(struct Owner08B2702C *self);

int func_08B2702C(struct Owner08B2702C *self)
{
    self->field_0->field_1B8 = 0;
    switch (self->field_0->field_1A8) {
    case 0:
        self->field_0->field_1A8 = 100;
    case 100:
        func_08B274C0(self);
        break;
    case 200:
        func_08B27818(self);
        break;
    case 300:
        break;
    case 10000:
        break;
    default:
        break;
    }
    func_08B253E4(self);
    func_08B2716C(self);
    return self->field_0->field_1B8;
}
