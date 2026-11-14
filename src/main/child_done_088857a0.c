typedef struct Owner088857A0 {
    unsigned char unknown_00[0x08];
    void *child;
} Owner088857A0;

int func_088860C8(void *child);

int func_088857A0(Owner088857A0 *self) {
    if (self->child != 0 && func_088860C8(self->child) == 0) {
        return 0;
    }
    return 1;
}
