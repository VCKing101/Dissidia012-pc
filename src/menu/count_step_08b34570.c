/* Advances the running counter; when it reaches its end, fires the end
   handler with the target value and resets the counter. */
struct Counter08B34570 {
    char pad_000[0x554];
    int field_554;
    int field_558;
    int field_55C;
    char pad_560[4];
    unsigned char field_564;
};

extern void func_08B34714(struct Counter08B34570 *self, float value);

void func_08B34570(struct Counter08B34570 *self)
{
    if (self->field_564 == 0) {
        return;
    }
    if (self->field_554 < self->field_558) {
        self->field_554 = (int)((float)self->field_554 + 1.0f);
    } else {
        func_08B34714(self, (float)self->field_55C);
        self->field_564 = 0;
        self->field_554 = 0;
        self->field_55C = -1;
        self->field_558 = -1;
    }
}
