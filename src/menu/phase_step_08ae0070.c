/* Runs one step of the screen's phase machine (phases 0, 100, 200, 300,
   400 and 10000), then the common update. Always returns 1. */
struct State08AE0070 {
    char pad_000[0x5A0];
    int field_5A0;
    unsigned int field_5A4;
};

struct Screen08AE0070 {
    char pad_000[0x850];
    struct State08AE0070 *field_850;
};

extern void func_08AE2904(struct Screen08AE0070 *self);
extern void func_08AE2164(struct Screen08AE0070 *self);
extern void func_08AE3008(struct Screen08AE0070 *self);
extern void func_08AE3388(struct Screen08AE0070 *self);
extern void func_08AE0638(struct Screen08AE0070 *self);
extern void func_08AE031C(struct Screen08AE0070 *self);

int func_08AE0070(struct Screen08AE0070 *self)
{
    func_08AE2904(self);
    func_08AE2164(self);
    switch (self->field_850->field_5A0) {
    case 0:
        self->field_850->field_5A0 = 100;
        if ((self->field_850->field_5A4 & 8) != 0) {
            self->field_850->field_5A0 = 400;
        }
    case 100:
        func_08AE3008(self);
        break;
    case 200:
        func_08AE3388(self);
        func_08AE0638(self);
        break;
    case 300:
        break;
    case 400:
        func_08AE3388(self);
        func_08AE0638(self);
        break;
    case 10000:
        break;
    default:
        break;
    }
    func_08AE031C(self);
    return 1;
}
