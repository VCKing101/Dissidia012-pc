typedef struct Owner0880FC58 {
    unsigned char unknown_0000[0x94];
    int field_0094;
    int handle;
    unsigned char unknown_009C[0x24];
    int field_00C0;
} Owner0880FC58;

void func_0886EB84(int handle);

void func_0880FC58(Owner0880FC58 *self) {
    if (self->handle != 0) {
        func_0886EB84(self->handle);
        self->handle = 0;
    }
    self->field_0094 = 0;
    self->field_00C0 = -1;
}
