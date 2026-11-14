/* Resets the state's counters and tables, then rebuilds the screen. */
struct State08B25070 {
    char pad_0000[0x1A8];
    int field_1A8;
    int field_1AC;
    char pad_01B0[0x1BC - 0x1B0];
    int field_1BC;
    char pad_01C0[0x1C8 - 0x1C0];
    float field_1C8;
    int field_1CC;
    char pad_01D0[4];
    int field_1D4;
    char pad_01D8[0x1E0 - 0x1D8];
    int field_1E0;
    int field_1E4[0x2E];
    char pad_029C[0x2FB4 - 0x29C];
    int field_2FB4[6];
};

struct Owner08B25070 {
    struct State08B25070 *field_0;
};

extern void func_08B27214(struct Owner08B25070 *self);
extern void func_08B25244(struct Owner08B25070 *self);
extern void func_08B26ED4(struct Owner08B25070 *self);
extern void func_08B26FB4(struct Owner08B25070 *self, int mode);

void func_08B25070(struct Owner08B25070 *self)
{
    int i;
    self->field_0->field_1AC = 0;
    self->field_0->field_1A8 = 0;
    self->field_0->field_1BC = 0;
    self->field_0->field_1C8 = 0.0f;
    self->field_0->field_1CC = 0;
    self->field_0->field_1D4 = 1;
    self->field_0->field_1E0 = 0;
    for (i = 0; i < 0x2E; i++) {
        self->field_0->field_1E4[i] = 0;
    }
    for (i = 0; i < 6; i++) {
        self->field_0->field_2FB4[i] = 0;
    }
    func_08B27214(self);
    func_08B25244(self);
    func_08B26ED4(self);
    func_08B26FB4(self, 0);
}
