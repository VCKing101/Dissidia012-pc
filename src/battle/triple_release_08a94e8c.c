typedef struct TripleHolder {
    char pad_00[0x4];
    void *field_04;
    void *field_08;
    char pad_0C[0x4];
    void *field_10;
} TripleHolder;

extern void func_08842138(void *object);
extern void func_08A94E70(TripleHolder *self);

void func_08A94E8C(TripleHolder *self)
{
    if (self->field_10 != 0) {
        func_08842138(self->field_10);
    }
    if (self->field_08 != 0) {
        func_08842138(self->field_08);
    }
    if (self->field_04 != 0) {
        func_08842138(self->field_04);
    }
    func_08A94E70(self);
}
