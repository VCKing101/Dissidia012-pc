typedef struct StreamSet {
    unsigned char active;
    char pad_01[0x3];
    int phase;
    char pad_08[0x14];
    char field_1C[0x24];
    char field_40[0x4];
} StreamSet;

extern int func_08B04F80(StreamSet *self);
extern int func_08B04FCC(StreamSet *self);
extern void func_08B05AD8(void *sub);
extern void func_08B056C4(void *sub, int phase);

int func_08B051DC(StreamSet *self)
{
    if (func_08B04F80(self) == 0) {
        return -1;
    }
    if (func_08B04FCC(self) == 0) {
        return 0;
    }
    func_08B05AD8(self->field_40);
    func_08B056C4(self->field_1C, self->phase);
    return 0;
}
