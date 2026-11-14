typedef struct Owner0888473C {
    unsigned char unknown_00[0xC];
    int slots[2];
} Owner0888473C;

int func_0888473C(Owner0888473C *self) {
    int result = 1;
    int i;
    for (i = 0; i < 2; i++) {
        if (self->slots[i] != 0) {
            result = 0;
            break;
        }
    }
    return result;
}
