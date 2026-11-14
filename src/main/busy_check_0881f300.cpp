struct Unit0881F300 {
    unsigned char active;
    unsigned char unknown_01[7];
    unsigned char field_08[0x1C];
    unsigned char field_24[4];
};

extern "C" {
int func_0881F82C(void *p);
int func_08820540(void *p);
}

extern "C" {
unsigned char func_0881F300(Unit0881F300 *self) {
    if (self->active == 0) {
        return 0;
    }
    return (func_0881F82C(self->field_08) != 0 || func_08820540(self->field_24) != 0) ? 1 : 0;
}
}
