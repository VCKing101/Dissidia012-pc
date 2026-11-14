/* Counter object: clears its slots, keeps the total and how many steps it takes. */
typedef struct StepCounter {
    void *field_00;
    int field_04[14];
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    unsigned char field_4C;
    char pad_4D[3];
    int field_50;
} StepCounter;

extern int D_089EC230;
extern void func_08935828(void *dst, int value, int size);

StepCounter *func_089EAFA0(StepCounter *self, int total, unsigned char flag, int step)
{
    int count;
    self->field_00 = &D_089EC230;
    self->field_3C = 0;
    self->field_4C = flag;
    self->field_50 = 0;
    func_08935828(self->field_04, 0, 0x38);
    self->field_40 = total;
    self->field_44 = 0;
    count = total / step;
    self->field_48 = count < 1 ? 1 : count;
    return self;
}
