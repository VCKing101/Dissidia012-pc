struct Quad16 { int a; int b; int c; int d; };
struct Pad {
    int field_00;
    Quad16 quads[8];
    int field_84;
    int field_88;
    unsigned char field_8C;
    unsigned char field_8D;
    char pad_8E[2];
    int field_90;
    int field_94[2];
    int field_9C[2];
    int field_A4[25];
    int field_108;
    int field_10C;
    unsigned char field_110;
    char pad_111[0xF];
    int field_120[2];
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
};
extern "C" void func_08869CA0(Pad *self)
{
    int none = -1;
    for (int i = 0; i < 8; i++) {
        Quad16 *q = &self->quads[i];
        q->a = 0;
        q->b = 0;
        q->c = 0;
        q->d = 0;
    }
    self->field_8C = 0;
    self->field_8D = none;
    self->field_84 = 0;
    self->field_88 = 0;
    self->field_108 = 0;
    self->field_10C = 0;
    for (int i = 0; i < 2; i++) {
        self->field_94[i] = 0;
        self->field_9C[i] = 0;
        self->field_120[i] = 0;
    }
    for (int i = 0; i < 25; i++) {
        self->field_A4[i] = none;
    }
    self->field_110 = 0;
    self->field_128 = 0;
    self->field_12C = 0;
    self->field_130 = 0;
    self->field_134 = 0;
    self->field_138 = 0;
    self->field_90 = 0;
}
