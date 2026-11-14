typedef struct Part088610D4 {
    int value;
    float scale;
    unsigned char unknown_08[0x10];
} Part088610D4;

typedef struct Owner088610D4 {
    unsigned char unknown_000[0x404];
    Part088610D4 parts[2];
} Owner088610D4;

int func_08861050(Owner088610D4 *self);

int func_088610D4(Owner088610D4 *self, int value, int index, float scale) {
    if (func_08861050(self) == 0) {
        return -1;
    }
    self->parts[index].value = value;
    self->parts[index].scale = scale;
    return 0;
}
