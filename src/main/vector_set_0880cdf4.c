typedef struct Vec0880CDF4 {
    unsigned char unknown_0000[0xC];
    float x;
    float y;
    float z;
} Vec0880CDF4;

void func_089A34C0();

void func_0880CDF4(Vec0880CDF4 *self, float x, float y, float z) {
    func_089A34C0();
    self->x = x;
    self->y = y;
    self->z = z;
}
