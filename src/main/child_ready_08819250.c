typedef struct Owner08819250 {
    unsigned char unknown_00[0xC];
    unsigned char active;
    unsigned char unknown_0D[7];
    void *child;
} Owner08819250;

int func_08815CE8(void *child);

int func_08819250(Owner08819250 *self) {
    if (self->active == 0) {
        return 0;
    }
    if (self->child == 0) {
        return 1;
    }
    return func_08815CE8(self->child);
}
