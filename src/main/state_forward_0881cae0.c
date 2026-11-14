typedef struct Agent4Forward {
    unsigned char unknown_0000[8];
    unsigned char mode;
    unsigned char unknown_0009[0x2B];
    unsigned char field_34[4];
} Agent4Forward;

int func_0881A2FC(void *part, int a, unsigned char b, unsigned char c);

int func_0881CAE0(Agent4Forward *self, int a, unsigned char b, unsigned char c) {
    if (self->mode == 1) {
        return 0;
    }
    return func_0881A2FC(self->field_34, a, b, c);
}
