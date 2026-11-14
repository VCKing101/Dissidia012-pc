typedef struct FieldBlock {
    char pad_00[0x8];
    unsigned char field_08;
    char pad_09[0x3];
    int field_0C;
    int field_10;
    int field_14[4];
    unsigned char field_24;
    char pad_25[0x3];
    float field_28;
    unsigned char field_2C;
    unsigned char field_2D;
    unsigned char field_2E;
    char pad_2F[0x1];
    int field_30;
    float field_34;
    float field_38;
    int field_3C;
    int field_40;
    char field_44[0x4];
} FieldBlock;

extern "C" {

extern void func_089EB804(void *sub);

void func_089EBC54(FieldBlock *self)
{
    self->field_08 = 0;
    self->field_0C = -1;
    self->field_10 = 0;
    for (int i = 0; i < 4; i++) {
        self->field_14[i] = 0;
    }
    self->field_24 = 0;
    self->field_28 = 0.0f;
    self->field_2C = 0;
    self->field_2D = 0;
    self->field_2E = 0;
    self->field_30 = -1;
    self->field_34 = 0.0f;
    self->field_38 = 0.0f;
    self->field_3C = 0;
    self->field_40 = 0;
    func_089EB804(self->field_44);
}

}
