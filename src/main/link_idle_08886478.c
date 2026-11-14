typedef struct Link08886478 {
    int handle;
    void *target;
} Link08886478;

int func_08886478(Link08886478 *self) {
    if (self->handle == 0) {
        return 1;
    }
    if (self->target != 0) {
        return 0;
    }
    return 1;
}
