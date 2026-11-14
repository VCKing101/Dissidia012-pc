typedef struct Entry08891684 {
    unsigned char valid;
    unsigned char unknown_01[0x18 - 1];
    int value;
} Entry08891684;

int func_08891684(Entry08891684 *self) {
    if (self->valid == 0) {
        return -1;
    }
    return self->value;
}
