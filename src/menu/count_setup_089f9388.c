/* Starts the count-up once: steps of 1 below 100, otherwise a ninth of the total plus one. */
typedef struct CountState {
    char pad_0000[0x273C];
    unsigned int field_273C;
    char pad_2740[0x3238 - 0x2740];
    unsigned int field_3238;
    char pad_323C[0x3268 - 0x323C];
    unsigned int field_3268;
    short field_326C;
    short field_326E;
    int field_3270;
} CountState;

typedef struct CountOwner {
    char pad_000[0x14C];
    CountState *field_14C;
} CountOwner;

extern int func_088D4BF4(int sound);
extern void func_089F9444(CountOwner *self);

void func_089F9388(CountOwner *self)
{
    short step;
    if ((self->field_14C->field_3238 & 0x40) != 0) {
        return;
    }
    self->field_14C->field_3268 = self->field_14C->field_273C;
    if (self->field_14C->field_273C == 0) {
        step = 1;
    } else if (self->field_14C->field_273C >= 100) {
        step = self->field_14C->field_273C / 9 + 1;
    } else {
        step = self->field_14C->field_273C;
    }
    self->field_14C->field_326C = step;
    self->field_14C->field_326E = 0;
    self->field_14C->field_3270 = func_088D4BF4(0x15);
    self->field_14C->field_3238 |= 0x40;
    func_089F9444(self);
}
