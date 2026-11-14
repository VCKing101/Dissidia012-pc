typedef struct SlotReset66390 {
    unsigned char active;
    unsigned char field_01;
    unsigned char field_02;
    char pad_003[0x1];
    char field_04[0x10];
    char field_14[0x4];
} SlotReset66390;

extern void func_08A7E8C8(void *sub, unsigned int mask);
extern void func_08A7E8A0(void *sub);

int func_08A66390(SlotReset66390 *self)
{
    if (!self->active) {
        return -1;
    }
    self->field_01 = 0;
    self->field_02 = 0;
    func_08A7E8C8(self->field_04, 0xFDFFFF7F);
    func_08A7E8A0(self->field_14);
    return 0;
}
