typedef struct Entry088914F0 {
    unsigned char valid;
    unsigned char unknown_01[0x1C - 1];
    int value;
} Entry088914F0;

int func_088914F0(Entry088914F0 *self) {
    if (self->valid == 0) {
        return -1;
    }
    return self->value;
}
