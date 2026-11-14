typedef struct Entry08891900 {
    unsigned char valid;
    unsigned char unknown_01[0x20 - 1];
    int value;
} Entry08891900;

int func_08891900(Entry08891900 *self) {
    if (self->valid == 0) {
        return -1;
    }
    return self->value;
}
