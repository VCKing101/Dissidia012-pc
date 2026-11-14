typedef struct StreamSet {
    unsigned char active;
    char pad_01[0x3];
    unsigned int phase;
    char pad_08[0x38];
    char field_40[0x4];
} StreamSet;

extern int func_08B04F80(StreamSet *self);
extern void func_08B056C4(void *sub, unsigned int phase);

int func_08B05040(StreamSet *self, unsigned int phase)
{
    if (func_08B04F80(self) == 0) {
        return -1;
    }
    if (phase >= 4) {
        return -2;
    }
    func_08B056C4(self->field_40, phase);
    self->phase = phase;
    return 0;
}
