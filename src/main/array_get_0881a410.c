typedef struct Agent4Array {
    unsigned char valid;
    unsigned char unknown_01[3];
    unsigned int count;
    int *items;
} Agent4Array;

int func_0881A410(Agent4Array *self, unsigned int index) {
    if (!self->valid) {
        return 0;
    }
    if (index >= self->count) {
        return 0;
    }
    return self->items[index];
}
