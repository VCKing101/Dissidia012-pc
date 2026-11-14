typedef struct Owner088857DC {
    unsigned char unknown_00[0x0C];
    void *child;
} Owner088857DC;

int func_08886478(void *child);

int func_088857DC(Owner088857DC *self) {
    if (self->child != 0 && func_08886478(self->child) == 0) {
        return 0;
    }
    return 1;
}
