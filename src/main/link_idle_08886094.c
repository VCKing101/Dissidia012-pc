typedef struct Link08886094 {
    int handle;
    void *target;
} Link08886094;

int func_08886094(Link08886094 *self) {
    if (self->handle == 0) {
        return 1;
    }
    if (self->target != 0) {
        return 0;
    }
    return 1;
}
