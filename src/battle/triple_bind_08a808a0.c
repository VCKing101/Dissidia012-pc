typedef struct TripleBind808A0 {
    unsigned char active;
    char pad_001[0x3];
    void *field_04;
    void *field_08;
    void *field_0C;
} TripleBind808A0;

extern void func_08A80884(TripleBind808A0 *self);

int func_08A808A0(TripleBind808A0 *self, void *a, void *b, void *c)
{
    func_08A80884(self);
    if (!a || !b || !c) {
        return -1;
    }
    self->field_04 = a;
    self->field_08 = b;
    self->field_0C = c;
    self->active = 1;
    return 0;
}
