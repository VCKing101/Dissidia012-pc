typedef struct Agent4Query {
    unsigned char valid;
    unsigned char unknown_01[7];
    unsigned char mode;
    unsigned char unknown_09[0xB];
    unsigned char field_14[4];
} Agent4Query;

int func_08816CF4(void *part, int key);

int func_0881C68C(Agent4Query *self, int key) {
    if (!self->valid) {
        return 0;
    }
    if (self->mode == 1) {
        return 0;
    }
    return func_08816CF4(self->field_14, key);
}
