typedef struct Owner08885818 {
    unsigned char unknown_00[0x08];
    void *child;
} Owner08885818;

int func_08886094(void *child);

int func_08885818(Owner08885818 *self) {
    if (self->child != 0 && func_08886094(self->child) == 0) {
        return 0;
    }
    return 1;
}
