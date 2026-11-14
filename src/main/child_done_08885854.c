typedef struct Owner08885854 {
    unsigned char unknown_00[0x04];
    void *child;
} Owner08885854;

int func_08884F24(void *child);

int func_08885854(Owner08885854 *self) {
    if (self->child != 0 && func_08884F24(self->child) == 0) {
        return 0;
    }
    return 1;
}
