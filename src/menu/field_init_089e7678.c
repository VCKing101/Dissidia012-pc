typedef struct InitBlock {
    char pad_000[0x134];
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
    int field_158;
    int field_15C;
    int field_160;
    int field_164;
    int field_168;
    int field_16C;
    int field_170;
    float field_174;
    int field_178;
    char field_17C;
    char pad_17D[3];
    char field_180[0x10];
    char field_190;
    char field_191;
} InitBlock;

extern void *func_08864FF0(void *dest, int count, int size, char flag);
extern void func_08935828(void *dest, char value, int size);

InitBlock *func_089E7678(InitBlock *self)
{
    func_08864FF0((char *)self + 0x100, 1, 0x11184, 0x41);
    self->field_134 = 0;
    self->field_138 = 0;
    self->field_13C = 0;
    self->field_140 = 0;
    self->field_144 = 0;
    self->field_148 = 0;
    self->field_190 = 0;
    self->field_191 = 0;
    self->field_150 = 0;
    self->field_14C = 0;
    self->field_154 = 0;
    self->field_158 = 0;
    self->field_15C = 0;
    self->field_160 = 0;
    self->field_164 = 0;
    self->field_168 = 0;
    self->field_16C = 0;
    self->field_170 = 0;
    self->field_174 = 0.0f;
    self->field_178 = 0;
    self->field_17C = 0;
    func_08935828(self->field_180, 0, 0x10);
    return self;
}
