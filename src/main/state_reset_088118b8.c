typedef struct State088118B8 {
    int field_00;
    int field_04;
    float field_08;
    unsigned char field_0C;
    unsigned char field_0D;
    unsigned char field_0E;
    unsigned char unknown_0F;
    int field_10;
    float field_14;
    unsigned int busy : 1;
    unsigned int queued : 1;
} State088118B8;

void func_088118B8(State088118B8 *self) {
    self->field_04 = 0;
    self->field_08 = 0.0f;
    self->field_0C = 0;
    self->field_0D = 0;
    self->field_10 = -1;
    self->field_14 = 0.0f;
    self->field_0E = 0;
    self->busy = 0;
    self->queued = 0;
}
