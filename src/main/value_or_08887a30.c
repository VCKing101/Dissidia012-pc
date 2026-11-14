typedef struct Owner08887A30 {
    int field_00;
    int field_04;
    int value;
} Owner08887A30;

int func_08887A30(Owner08887A30 *self, int fallback) {
    int value = self->value;
    if (value == 0) {
        return fallback;
    }
    return value;
}
