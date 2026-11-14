typedef struct ChildSelect222C8 {
    unsigned char active;
    char pad_001[0x3];
    char field_04[0xFC];
    char field_100[0x4];
} ChildSelect222C8;

extern void func_08A07394(void *sub, unsigned int index);
extern void func_08A2399C(void *sub);

int func_08A222C8(ChildSelect222C8 *self, unsigned int index)
{
    if (!self->active) {
        return -1;
    }
    if (index >= 2) {
        return -2;
    }
    func_08A07394(self->field_100, index);
    func_08A2399C(self->field_04);
    return 0;
}
