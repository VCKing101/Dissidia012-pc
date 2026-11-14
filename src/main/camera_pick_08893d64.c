typedef struct Owner08893D64 {
    unsigned char unknown_000[0x4E0];
    unsigned char field_4E0[0x84];
    unsigned char field_564;
    unsigned char unknown_565[0xAB];
    unsigned char field_610;
} Owner08893D64;

void func_088953BC(void *camera);

void func_08893D64(Owner08893D64 *self) {
    if (self->field_610 == 1 && self->field_564 == 1) {
        func_088953BC(self->field_4E0);
    } else {
        func_088953BC(self);
    }
}
