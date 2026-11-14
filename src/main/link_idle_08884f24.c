typedef struct Link08884F24 {
    int field_00;
    int target;
    int field_08;
    int request;
} Link08884F24;

int func_08884F24(Link08884F24 *self) {
    if (self->request != 0) {
        if (self->target != 0) {
            return 0;
        }
        return 1;
    }
    return 1;
}
