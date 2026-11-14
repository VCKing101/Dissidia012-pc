typedef struct QuadSlot {
    unsigned char field_00;
    char pad_01[0x7];
    char field_08[0x60];
    void *field_68;
    void *field_6C;
} QuadSlot;

extern void func_08A25BDC(QuadSlot *self);
extern int func_089F7D74(void *slot, void *data, void *extra);

int func_08A25C8C(QuadSlot *self, void *first, void *second, void *data, void *extra)
{
    func_08A25BDC(self);
    if (first == 0 || second == 0 || data == 0 || extra == 0) {
        return -1;
    }
    func_089F7D74(self->field_08, data, extra);
    self->field_68 = first;
    self->field_6C = second;
    self->field_00 = 1;
    return 0;
}
