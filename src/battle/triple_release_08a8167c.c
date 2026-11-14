typedef struct TripleHolder {
    char pad_00[0x4];
    void *field_04;
    void *field_08;
    void *field_0C;
} TripleHolder;

extern void func_08842138(void *object);
extern void func_08A81668(TripleHolder *self);

void func_08A8167C(TripleHolder *self)
{
    if (self->field_0C != 0) {
        func_08842138(self->field_0C);
    }
    if (self->field_08 != 0) {
        func_08842138(self->field_08);
    }
    if (self->field_04 != 0) {
        func_08842138(self->field_04);
    }
    func_08A81668(self);
}
