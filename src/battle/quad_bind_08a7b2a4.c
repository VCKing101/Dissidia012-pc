typedef struct QuadBind7B2A4 {
    unsigned char active;
    char pad_001[0x3];
    void *field_04;
    void *field_08;
    void *field_0C;
    void *field_10;
} QuadBind7B2A4;

extern void func_08A7B288(QuadBind7B2A4 *self);

int func_08A7B2A4(QuadBind7B2A4 *self, void *a, void *b, void *c, void *d)
{
    func_08A7B288(self);
    if (!a || !b || !c || !d) {
        return -1;
    }
    self->field_04 = a;
    self->field_08 = b;
    self->field_0C = c;
    self->field_10 = d;
    self->active = 1;
    return 0;
}
