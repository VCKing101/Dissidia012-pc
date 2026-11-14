typedef struct MenuSlotBuffer {
    int field_00;
    char field_04[0x900];
    int field_904;
    int field_908;
    unsigned char field_90C;
} MenuSlotBuffer;

extern void func_08935828(void *dst, int value, int size);

void func_08A3A8A8(MenuSlotBuffer *self) {
    self->field_00 = 0;
    self->field_904 = 0;
    self->field_908 = 0;
    func_08935828(self->field_04, 0, 0x900);
    self->field_90C = 0;
}
