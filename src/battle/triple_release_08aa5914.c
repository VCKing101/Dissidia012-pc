typedef struct TripleHolder {
    char pad_00[0x8];
    void *field_08;
    char pad_0C[0x24];
    void *field_30;
    char pad_34[0x8];
    void *field_3C;
} TripleHolder;

extern void func_08842138(void *object);
extern void func_08AA58A4(TripleHolder *self);

void func_08AA5914(TripleHolder *self)
{
    if (self->field_3C != 0) {
        func_08842138(self->field_3C);
    }
    if (self->field_30 != 0) {
        func_08842138(self->field_30);
    }
    if (self->field_08 != 0) {
        func_08842138(self->field_08);
    }
    func_08AA58A4(self);
}
