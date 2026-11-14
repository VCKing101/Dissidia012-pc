typedef struct TripleSlot18E80 {
    unsigned char field_00;
    char pad_01[0x13];
    void *field_14;
    void *field_18;
    void *field_1C;
} TripleSlot18E80;

extern void func_08A18E18(TripleSlot18E80 *self);

int func_08A18E80(TripleSlot18E80 *self, void *first, void *second, void *third)
{
    func_08A18E18(self);
    if (first == 0 || second == 0 || third == 0) {
        return -1;
    }
    self->field_14 = first;
    self->field_18 = second;
    self->field_1C = third;
    self->field_00 = 1;
    return 0;
}
