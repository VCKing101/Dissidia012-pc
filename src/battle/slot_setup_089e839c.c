typedef struct SlotSetup {
    char pad_00[0x8];
    unsigned char ready;
    char pad_09[0x3];
    int state;
    char pad_10[0x24];
    void *field_34;
    void *field_38;
    unsigned int count;
    void *field_40;
    int field_44;
    int field_48;
    float field_4C;
} SlotSetup;

extern void func_089E831C(SlotSetup *self);

int func_089E839C(SlotSetup *self, void *a, void *b, unsigned int count, void *d, int e, int f, float g)
{
    func_089E831C(self);
    if (a == 0 || b == 0 || count > 16 || d == 0) {
        return -1;
    }
    self->field_34 = a;
    self->field_38 = b;
    self->count = count;
    self->field_40 = d;
    self->field_44 = e;
    self->field_48 = f;
    self->field_4C = g;
    self->state = 0;
    self->ready = 1;
    return 0;
}
