typedef struct Agent4ValueHolder {
    unsigned char valid;
    unsigned char unknown_01[3];
    int value;
} Agent4ValueHolder;

int func_0881C468(Agent4ValueHolder *self, int value) {
    if (!self->valid) {
        return -1;
    }
    self->value = value;
    return 0;
}
