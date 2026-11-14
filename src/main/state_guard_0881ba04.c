typedef struct Agent4Ready {
    unsigned char valid;
    unsigned char unknown_01[0xF];
    unsigned char field_10;
} Agent4Ready;

int func_0881BA04(Agent4Ready *self) {
    if (!self->valid) {
        return -1;
    }
    self->field_10 = 1;
    return 0;
}
