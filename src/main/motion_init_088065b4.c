typedef struct Motion088065B4 {
    unsigned char unknown_0000[0x690];
    int field_0690;
    float field_0694;
    float field_0698;
    float field_069C;
    float field_06A0;
    float field_06A4;
    float field_06A8;
    float field_06AC;
    unsigned char unknown_06B0[8];
    short field_06B8;
    unsigned char field_06BA;
    unsigned char field_06BB;
} Motion088065B4;

void func_088065B4(Motion088065B4 *self, int id, int kind, float value) {
    float zero = 0.0f;
    self->field_06BB = 0;
    self->field_069C = zero;
    self->field_06A4 = zero;
    self->field_06A0 = zero;
    self->field_06AC = zero;
    self->field_06A8 = zero;
    self->field_0694 = value;
    self->field_0698 = zero;
    self->field_06B8 = kind;
    self->field_0690 = id;
    self->field_06BA = 1;
}
