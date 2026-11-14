typedef struct Part08860F80 {
    unsigned char data[0x18];
} Part08860F80;

typedef struct Owner08860F80 {
    unsigned char field_000;
    unsigned char field_001;
    unsigned char unknown_002[0x402];
    Part08860F80 parts[2];
    float field_434;
    float field_438;
} Owner08860F80;

extern "C" void func_088611E8(Part08860F80 *part);

extern "C" void func_08860F80(Owner08860F80 *self) {
    self->field_000 = 0;
    self->field_001 = 0;
    for (int i = 0; i < 2; i++) {
        func_088611E8(&self->parts[i]);
    }
    self->field_434 = 0.0f;
    self->field_438 = 0.0f;
}
